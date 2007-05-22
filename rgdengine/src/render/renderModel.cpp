#include "precompiled.h"

#include <rgde/render/renderModel.h>
#include <rgde/render/renderLight.h>

#include <rgde/base/xml_helpers.h>

using namespace std;
using math::Color;

namespace render
{
	CModel::CModel() : m_bVisible(true)
	{
	}

	CModel::~CModel()
	{
		clear();
	}

	void ReadNode(TiXmlElement *elem, math::CFrame &rootFrame, CModel &model);
	CMesh::PGeometry ReadGeometry(const std::string& fNm);

	PModel CModel::Create(const std::string& strFileName)
	{
		//try
		{
			PModel pModel = new CModel();
			pModel->load(strFileName);
            pModel->setLooped(true);
			pModel->play();
			pModel->update(0.0001f);			
			return pModel;
		}
		//catch (...)
		//{
		//	base::lwrn << "Fail to load model: " << strFileName;
		//}

		return 	PModel();
	}

	void CModel::load(const std::string& strModelName)
	{
		base::lmsg << "loading model: " << "\"" << strModelName << "\"";

		io::CFileSystem &fs	= io::TheFileSystem::Get();

		io::ScopePathAdd p	("Models/" + strModelName + "/");
		io::PReadStream in	= fs.findFile(strModelName + ".xml");

		if (!in)
		{
			std::string error	= "CModel::load: can't load file ";// + strFileName;
			base::lerr << "CModel::load: can't load file: " << strModelName << ".xml";
			throw exception(error.c_str());
		}

		std::vector<byte> data;
		io::StreamToVector(data, in);

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
				m_vMaterials[m_id] = CMaterial::Create(str);
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

	void ReadNode(TiXmlElement *elem, math::CFrame &rootFrame, CModel &model)
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
			math::CFrameAnimationController control;
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
			PMesh pMesh(new CMesh);
			std::string strMeshFile	= std::string(gm->Attribute("name")) + ".xml";

			pMesh->load(strMeshFile);

			if (m_id >= 0)
				pMesh->getMaterials().push_back(model.getMaterials()[m_id]);

			model.getMeshes().push_back(pMesh);

			rootFrame.addChild(pMesh.get());
			// читаем всех детей

			//Neonic: octree
			if(bDynamic)
				pMesh->makeDynamic();
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
				CPointLight *pPointLight = new CPointLight(rootFrame.getName());
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
						range = base::Lexical_cast<float, std::string>(att->Attribute("end"));

				pPointLight->setRange(range);
				pPointLight->setConstantAttenuation(0.0f);
				pPointLight->setLinearAttenuation(1.0f);
				pPointLight->setQuadraticAttenuation(0.0f);

				std::string strEnabled = "1";

				TiXmlElement *enabled = lt->FirstChildElement("enabled");
				if(enabled)
					if(enabled->Attribute("value"))
						strEnabled = enabled->Attribute("value");

				pPointLight->setEnabled(base::Lexical_cast<int, std::string>(strEnabled));
			}
			else
				base::lwrn<<"\""<<strType<<"\" light type temporally unsupported.";
		}


		TiXmlNode *cd	= 0;
		while (cd = elem->IterateChildren("node", cd))
		{
			math::PFrame child	= new math::CFrame;
			ReadNode(cd->ToElement(), *(child.get()), model);
			rootFrame.addChild(child);
			model.getFrames().push_back(child); // а это нафига??
		};
	}

	CMesh::PGeometry ReadGeometry(std::string fNm)
	{
		return CMesh::PGeometry();
	}

	unsigned int CModel::getFaceNum() const
	{
		unsigned int ret= 0;
		Meshes::const_iterator i;
		for (i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
			ret += (*i)->getNumPrimitives();
		return ret;
	}

	unsigned int CModel::getVertexNum() const
	{
		unsigned int ret= 0;
		Meshes::const_iterator i;
		for (i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
			ret += (*i)->getVertexNum();
		return ret;
	}

	void CModel::clear( void )
	{
		m_vMaterials.clear();
		m_vFrames.clear();
		Meshes::const_iterator i;
		for (Meshes::const_iterator i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i)
			(*i)->eject();
		m_vMeshes.clear();
		m_vControllers.clear();
	};

	void CModel::update(float dt)
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).update(dt);
	}

	//Neonic: octree
	void CModel::updateTree( void )
	{
		Meshes::const_iterator i;
		for ( i = m_vMeshes.begin(); i != m_vMeshes.end(); ++i )
			( *i )->updateTree();
	};

	void CModel::play()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).start();
	}

	void CModel::stop()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).stop();
	}

	void CModel::pause()
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).pause();
	}

	void CModel::setLooped(bool flag)
	{
		Controllers::iterator i;
		for (i = m_vControllers.begin(); i != m_vControllers.end(); ++i)
			(*i).setLooped(flag);
	}

	bool CModel::isVisible() const
	{
		return m_bVisible;
	}

	void CModel::setVisible(bool bVisible)
	{
		if (bVisible == m_bVisible)
			return;

		m_bVisible = bVisible;

		size_t nNumMeshes	= m_vMeshes.size();

		for (size_t i = 0; i < nNumMeshes; i++)
			if (m_bVisible)
				m_vMeshes[i]->show();
			else
				m_vMeshes[i]->hide();
	}
}