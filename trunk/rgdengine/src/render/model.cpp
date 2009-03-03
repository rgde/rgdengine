#include "precompiled.h"

#include <rgde/render/model.h>
#include <rgde/render/light.h>

#include <rgde/base/xml_helpers.h>

using namespace std;
using math::Color;

namespace render
{
	Model::Model() : m_is_visible(true)
	{
	}

	Model::~Model()
	{
		clear();
	}

	void ReadNode(TiXmlElement *elem, math::Frame &rootFrame, Model &model);
	Mesh::PGeometry ReadGeometry(const std::string& fNm);

	PModel Model::create(const std::string& file_name)
	{
		//try
		{
			PModel pModel = new Model();
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

		return 	PModel();
	}

	void Model::load(const std::string& strModelName)
	{
		base::lmsg << "loading model: " << "\"" << strModelName << "\"";

		io::CFileSystem &fs	= io::TheFileSystem::get();

		io::ScopePathAdd p	("Models/" + strModelName + "/");
		io::readstream_ptr in	= fs.findFile(strModelName + ".xml");

		if (!in)
		{
			std::string error	= "Model::load: can't load file ";// + file_name;
			base::lerr << "Model::load: can't load file: " << strModelName << ".xml";
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
				m_vMaterials[m_id] = Material::create(str);
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

	void ReadNode(TiXmlElement *elem, math::Frame &rootFrame, Model &model)
	{
		if (elem->Attribute("name"))
		{
			rootFrame.setName(elem->Attribute("name"));
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
			math::FrameAnimationController control;
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
			PMesh pMesh(new Mesh);
			std::string strMeshFile	= std::string(gm->Attribute("name")) + ".xml";

			pMesh->load(strMeshFile);

			if (m_id >= 0)
				pMesh->getMaterials().push_back(model.getMaterials()[m_id]);

			model.getMeshes().push_back(pMesh);

			rootFrame.addChild(pMesh.get());
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
				PointLight *pPointLight = new PointLight(rootFrame.getName());
				rootFrame.addChild(pPointLight);

				math::Color color;

				base::readLightColor(color, lt, "ambient");
				pPointLight->setAmbient(color);

				base::readLightColor(color, lt, "diffuse");
				pPointLight->setDiffuse(color);

				base::readLightColor(color, lt, "spec");
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
			math::frame_ptr child	= new math::Frame;
			ReadNode(cd->ToElement(), *(child.get()), model);
			rootFrame.addChild(child);
			model.getFrames().push_back(child); // а это нафига??
		};
	}

	Mesh::PGeometry ReadGeometry(std::string fNm)
	{
		return Mesh::PGeometry();
	}

	unsigned int Model::getFaceNum() const
	{
		unsigned int ret= 0;
		Meshes::const_iterator i;
		for (i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
			ret += (*i)->getNumPrimitives();
		return ret;
	}

	unsigned int Model::getVertexNum() const
	{
		unsigned int ret= 0;
		Meshes::const_iterator i;
		for (i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
			ret += (*i)->getVertexNum();
		return ret;
	}

	void Model::clear( void )
	{
		m_vMaterials.clear();
		m_vFrames.clear();
		Meshes::const_iterator i;
		//for (Meshes::const_iterator i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
		//	(*i)->eject();
		m_vMeshes.clear();
		m_vControllers.clear();
	};

	void Model::update(float dt)
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).update(dt);
	}

	//Neonic: octree
	void Model::updateTree( void )
	{
		Meshes::const_iterator i;
		for ( i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i )
			( *i )->updateTree();
	};

	void Model::play()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).start();
	}

	void Model::stop()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).stop();
	}

	void Model::pause()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).pause();
	}

	void Model::setLooped(bool flag)
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).setLooped(flag);
	}

	bool Model::isVisible() const
	{
		return m_is_visible;
	}

	void Model::setVisible(bool bVisible)
	{
		if (bVisible == m_is_visible)
			return;

		m_is_visible = bVisible;

		size_t nNumMeshes	= m_vMeshes.size();

		for (size_t i = 0; i < nNumMeshes; i++)
			if (m_is_visible)
				m_vMeshes[i]->show();
			else
				m_vMeshes[i]->hide();
	}
}