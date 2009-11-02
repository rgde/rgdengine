#include "precompiled.h"

#include <rgde/render/model.h>
#include <rgde/render/light.h>

#include <rgde/base/xml_helpers.h>

using namespace std;
using math::Color;

namespace render
{
	model::model() : m_is_visible(true)
	{
	}

	model::~model()
	{
		clear();
	}

	void ReadNode(TiXmlElement *elem, math::frame &rootFrame, model &model);
	mesh::PGeometry ReadGeometry(const std::string& fNm);

	model_ptr model::create(const std::string& file_name)
	{
		//try
		{
			model_ptr pModel = new model();
			pModel->load(file_name);
            pModel->setLooped(true);
			pModel->play();
			pModel->update(0.0001f);			
			return pModel;
		}
		//catch (...)
		//{
		//	base::lwrn << "Fail to load model: " << file_name;
		//}

		return 	model_ptr();
	}

	void model::load(const std::string& strModelName)
	{
		base::lmsg << "loading model: " << "\"" << strModelName << "\"";

		io::CFileSystem &fs	= io::TheFileSystem::get();

		io::path_add_scoped p	("Models/" + strModelName + "/");
		io::readstream_ptr in	= fs.find(strModelName + ".xml");

		if (!in)
		{
			std::string error	= "model::load: can't load file ";// + file_name;
			base::lerr << "model::load: can't load file: " << strModelName << ".xml";
			throw exception(error.c_str());
		}

		std::vector<byte> data;
		io::stream_to_vector(data, in);

		TiXmlDocument xml;//(std::string(strModelName.begin(), strModelName.end()));
		xml.Parse((const char*)&(data[0]));

		clear();

		TiXmlElement *mod_elem	= xml.RootElement();
		TiXmlElement *elem		= mod_elem->FirstChild("material_list")->ToElement();

		if (elem)
		{
			for (TiXmlNode* pNode = elem->IterateChildren(NULL);
					pNode != 0; pNode = elem->IterateChildren(pNode))
			{
				TiXmlElement *tx= pNode->ToElement();
				int m_id= 0; 
				tx->Attribute("id", &m_id);

				std::string str	= tx->Attribute("file");
				m_materials[m_id] = material::create(str);
			}
		}
		{
			TiXmlNode *elem = mod_elem->FirstChild("node");

			if (elem)
				ReadNode(elem->ToElement(), *this, *this);
		}

		//Neonic: octree. Здесь обновляем дерево для всей модели.
		updateTree();
	}

	void ReadNode(TiXmlElement *elem, math::frame &rootFrame, model &model)
	{
		if (elem->Attribute("name"))
		{
			rootFrame.set_name(elem->Attribute("name"));
		}

		//////////////////////////////////////////////////////////////////////////
		// load transformation data if exist
		{
			// translation
			if (TiXmlElement * pNode = elem->FirstChildElement("translation"))
			{
				math::Vec3f v;
				base::read(v, pNode);
				rootFrame.setPosition(v);
			}

			//rotation
			if (TiXmlElement * pNode = elem->FirstChildElement("rotation"))
			{
				math::Vec3f v;
				base::read(v, pNode);

				using math::Math::deg2Rad;				
				math::EulerAngleXYZf ang(deg2Rad(v[0]), deg2Rad(v[1]), deg2Rad(v[2]));

				math::Quatf q;
				math::set(q, ang);

				rootFrame.setRotation(q);
			}

			//scale
			if (TiXmlElement * pNode = elem->FirstChildElement("scale"))
			{
				math::Vec3f v;
				base::read(v, pNode);
				rootFrame.setScale(v);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		TiXmlElement *gm	= elem->FirstChildElement("animation");

		//Neonic: octree
		bool bDynamic = 0;

		if (gm)
		{
			math::frame_anim_controller control;
			control.load(elem);

			control.atachToFrame(&rootFrame);
			model.getControllers().push_back(control);

			//Neonic: octree
			bDynamic=1;
		}

		TiXmlElement *mt= elem->FirstChildElement("material");

		int m_id		= -1;

		if (mt)
			mt->Attribute("id", &m_id); 		   

		gm = elem->FirstChildElement("geometry");

		if (gm)
		{
			mesh_ptr pMesh(new mesh);
			std::string strMeshFile	= std::string(gm->Attribute("name")) + ".xml";

			pMesh->load(strMeshFile);

			if (m_id >= 0)
				pMesh->getMaterials().push_back(model.getMaterials()[m_id]);

			model.getMeshes().push_back(pMesh);

			rootFrame.add(pMesh.get());
			// читаем всех детей

			//Neonic: octree
			//if(bDynamic)
			//	pMesh->makeDynamic();
		}

		//By PC
		TiXmlElement *lt= elem->FirstChildElement("light");

		if (lt)
		{
			std::string strType = "point";

			TiXmlElement *type = lt->FirstChildElement("type");
			if(type)
				if(type->Attribute("value"))
					strType = type->Attribute("value");

			if(strType == "point")
			{
				PointLight *pPointLight = new PointLight(rootFrame.get_name());
				rootFrame.add(pPointLight);

				math::Color color;

				base::read_light_color(color, lt, "ambient");
				pPointLight->setAmbient(color);

				base::read_light_color(color, lt, "diffuse");
				pPointLight->setDiffuse(color);

				base::read_light_color(color, lt, "spec");
				pPointLight->setSpecular(color);

				float range = 1.0f;

				TiXmlElement *att = lt->FirstChildElement("far_att");
				if(att)
					if(att->Attribute("end"))
						range = base::lexical_cast<float, std::string>(att->Attribute("end"));

				pPointLight->setRange(range);
				pPointLight->setConstantAttenuation(0.0f);
				pPointLight->setLinearAttenuation(1.0f);
				pPointLight->setQuadraticAttenuation(0.0f);

				std::string strEnabled = "1";

				TiXmlElement *enabled = lt->FirstChildElement("enabled");
				if(enabled)
					if(enabled->Attribute("value"))
						strEnabled = enabled->Attribute("value");

				pPointLight->setEnabled(base::lexical_cast<int, std::string>(strEnabled)>0);
			}
			else
				base::lwrn<<"\""<<strType<<"\" light type temporally unsupported.";
		}


		TiXmlNode *cd	= 0;
		while (cd = elem->IterateChildren("node", cd))
		{
			math::frame_ptr child	= new math::frame;
			ReadNode(cd->ToElement(), *(child.get()), model);
			rootFrame.add(child);
			model.getFrames().push_back(child); // а это нафига??
		};
	}

	mesh::PGeometry ReadGeometry(std::string fNm)
	{
		return mesh::PGeometry();
	}

	unsigned int model::getFaceNum() const
	{
		unsigned int ret= 0;
		meshes_vector::const_iterator i;
		for (i = m_meshes.begin(); i != m_meshes.end(); ++i)
			ret += (*i)->getNumPrimitives();
		return ret;
	}

	unsigned int model::get_num_verts() const
	{
		unsigned int ret= 0;
		meshes_vector::const_iterator i;
		for (i = m_meshes.begin(); i != m_meshes.end(); ++i)
			ret += (*i)->get_num_verts();
		return ret;
	}

	void model::clear( void )
	{
		m_materials.clear();
		m_frames.clear();
		meshes_vector::const_iterator i;
		//for (meshes_vector::const_iterator i = m_meshes.begin(); i != m_meshes.end(); ++i)
		//	(*i)->eject();
		m_meshes.clear();
		m_controllers.clear();
	};

	void model::update(float dt)
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).update(dt);
	}

	//Neonic: octree
	void model::updateTree( void )
	{
		meshes_vector::const_iterator i;
		for ( i = m_meshes.begin(); i != m_meshes.end(); ++i )
			( *i )->updateTree();
	};

	void model::play()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).start();
	}

	void model::stop()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).stop();
	}

	void model::pause()
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).pause();
	}

	void model::setLooped(bool flag)
	{
		contollers_vector::iterator i;
		for (i = m_controllers.begin(); i != m_controllers.end(); ++i)
			(*i).setLooped(flag);
	}

	bool model::isVisible() const
	{
		return m_is_visible;
	}

	void model::setVisible(bool bVisible)
	{
		if (bVisible == m_is_visible)
			return;

		m_is_visible = bVisible;

		size_t nNumMeshes	= m_meshes.size();

		for (size_t i = 0; i < nNumMeshes; i++)
			if (m_is_visible)
				m_meshes[i]->show();
			else
				m_meshes[i]->hide();
	}
}