#include "precompiled.h"

#include "render/renderMaterial.h"
#include "render/renderManager.h"
#include "render/renderBinders.h"

#include "base/xml_helpers.h"

namespace render
{
	void CMaterial::CMaterialMap::setTexture(const PTexture& pTexture)
	{
		if (pTexture != NULL)
		{
			m_pTexture = pTexture;
			m_bTextureIsValid = true;
		}
		else
		{
			m_pTexture = m_pDefaultTexture;
			m_bTextureIsValid = false;
			//m_bTextureIsValid = true;
		}
	}

	void CMaterial::CMaterialMap::update(float fDt)
	{
		m_fTime += fDt;

		math::Matrix44f tempMatrix;

		math::setTrans(tempMatrix, m_scrollSpeed * m_fTime);
		m_matrix = tempMatrix;

		math::setTrans(tempMatrix, m_rotationCenter);
		m_matrix *= tempMatrix;

		math::setRot(tempMatrix, math::AxisAnglef(m_fRotationSpeed * m_fTime, math::Vec3f(0.0f, 0.0f, 1.0f)));//Is it right? (rotating around Z axis)
		m_matrix *= tempMatrix;

		math::setTrans(tempMatrix, -m_rotationCenter);
		m_matrix *= tempMatrix;
	}

	PTexture& getDefaultTextureByType(CMaterial::CMaterialMap::EDefaultTexture defaultTexture)
	{
		switch (defaultTexture)
		{
		case CMaterial::CMaterialMap::Black:
			return TheRenderManager::Get().getBlackTexture();
			break;

		case CMaterial::CMaterialMap::White:
			return TheRenderManager::Get().getWhiteTexture();
			break;

		case CMaterial::CMaterialMap::DefaultNormalMap:
			return TheRenderManager::Get().getDefaultNormalMap();
			break;
		}
	}

	CMaterial::CMaterialMap::CMaterialMap(EDefaultTexture defaultTexture)
	{
		m_pDefaultTexture = getDefaultTextureByType(defaultTexture);
		PTexture pNullTexture;
		setTexture(pNullTexture);

		m_fRotationSpeed = 0.0f;
		m_fTime = 0.0;
	}

	PMaterial CMaterial::Create(math::Color amb, math::Color diff, math::Color spec, math::Color em, float power)
	{
		return PMaterial(new CMaterial(amb, diff, spec, em, power));
	}

	PMaterial CMaterial::Create(const std::string& fileName)
	{
		PMaterial mat = CMaterial::Create();
		mat->load(fileName);
		return mat;
	}

	CMaterial::CMaterial(math::Color amb, math::Color diff, math::Color spec, math::Color em, float power)
		: m_ambient(amb),
		  m_diffuse(diff),
		  m_specular(spec),
		  m_emissive(em),
		  m_fPower(power)
	{
		m_maps["diffuse"] = CMaterialMap();
		m_maps["illumination"] = CMaterialMap(CMaterialMap::Black);
		m_maps["reflection"] = CMaterialMap(CMaterialMap::Black);
		m_maps["bump"] = CMaterialMap(CMaterialMap::DefaultNormalMap);
		m_maps["lightmap"] = CMaterialMap();
		//base::lmsg << "CMaterial::CMaterial()";
	}

	CMaterial::~CMaterial()
	{
		//base::lmsg << "CMaterial::~CMaterial()";
	}

	bool CMaterial::isTransparent() const
	{
		bool texHasAlpha	= false;
		MaterialMaps::const_iterator it = m_maps.find("diffuse");
		if(it->second.isTextureValid())
			texHasAlpha = it->second.getTexture()->hasAlpha();

		if (m_diffuse.a == 255 && !texHasAlpha)
			return false;
		else
			return true;
	}

	int getMapId(TiXmlElement *elem)
	{
		int id					= -1;
		TiXmlElement *pMapElem	= elem->FirstChildElement("map");
		if (NULL != pMapElem)
			pMapElem->Attribute("id", &id);

		return id;
	}

	TiXmlElement * assignMap(TiXmlElement *elem, std::string name, std::vector<PTexture> &textures, CMaterial::MaterialMaps &maps)
	{
		TiXmlElement *pMapEl= elem->FirstChildElement(name);
		if (NULL == pMapEl)
			return NULL;

		int id	= getMapId(pMapEl);

		if (id >= 0)
			maps[name].setTexture(textures[id]);

		return pMapEl;
	}

	float readFloatValue(TiXmlElement *elem, std::string name, float default_value = 0.0f)
	{
		double value= default_value;

		if (NULL == elem)
			return 0;

		TiXmlElement *child	= elem->FirstChildElement(name);

		if (NULL == child)
			return 0;

		child->Attribute("value", &value);
		return (float)value;
	}

	void loadMaterialTextures(TiXmlElement *elem, std::vector<PTexture> &textures)
	{
		TiXmlElement *maps	= elem->FirstChildElement("maps");
		if (maps)
		{
			TiXmlNode* pNode = NULL;
			while (pNode = maps->IterateChildren(pNode))
			{
				TiXmlElement *tx = pNode->ToElement();
				if (0 != tx->Attribute("file"))
				{
					std::string str	= tx->Attribute("file");
					PTexture tex	= ITexture::Create(str);
					textures.push_back(tex);
				}
				else
				{
					textures.push_back(PTexture());
				}
			};
		}
	}

	void CMaterial::load(const std::string& fileName)
	{
		m_file_name = fileName;
		base::lmsg << "loading material: " << "\"" << fileName << "\"";

		TiXmlDocument mat;
		{
			io::ScopePathAdd p	("Materials/");
			if (!base::loadXml(fileName, mat))
			{
				base::lerr << "Не могу загрузить файл материала!" << "\"" << fileName << "\"";
				return;
			}
		}

		TiXmlElement *elem	= mat.RootElement();

		std::vector<PTexture> textures;
		loadMaterialTextures(elem, textures);

		float a		= readFloatValue(elem, "opacity", 1.0f);
		float illum	= readFloatValue(elem, "illumination");
		
		m_emissive.r = m_emissive.g = m_emissive.b = static_cast<uchar>(illum*255);
		m_emissive.a = static_cast<uchar>(a*255);

		TiXmlElement *pAmbientEl= assignMap(elem, "ambient", textures, getMaterialMaps());
		base::readColorFromXml(m_ambient, pAmbientEl, a);

		TiXmlElement *pDiffEl	= assignMap(elem, "diffuse", textures, getMaterialMaps());
		base::readColorFromXml(m_diffuse, pDiffEl, a);

		TiXmlElement *pSpecEl	= assignMap(elem, "specular", textures, getMaterialMaps());
		base::readColorFromXml(m_specular, pSpecEl, a);

		//I didn't know how it works so I made as knew :)
		assignMap(elem, "illumination", textures, getMaterialMaps());
		assignMap(elem, "reflection", textures, getMaterialMaps());
		assignMap(elem, "bump", textures, getMaterialMaps());
		assignMap(elem, "lightmap", textures, getMaterialMaps());

		m_fPower = 255.0f;
	}

	const CMaterial::CMaterialMap & CMaterial::getMaterialMap(const std::string &type) const
	{
		MaterialMaps::const_iterator it	= m_maps.find(type);
		return it->second;
	}

	CMaterial::CMaterialMap & CMaterial::getMaterialMap(const std::string &type)
	{
		MaterialMaps::iterator it	= m_maps.find(type);
		return it->second;
	}

	const PTexture & CMaterial::getTextureMap(const std::string &type) const
	{
		return getMaterialMap(type).getTexture();
	}

	void CMaterial::update(float fDt)
	{
		MaterialMaps::iterator it	= m_maps.begin();
		for (; it != m_maps.end(); it++)
			it->second.update(fDt);
	}

	void CMaterial::setEffect(const PEffect& pEffect)
	{
		if(m_technique)
			m_technique = NULL;
		if(m_pDynamicBinder)
			m_pDynamicBinder = PDynamicBinder();

		std::string techName;

		m_pDynamicBinder = createDynamicBinder(pEffect, *this, techName);

		m_technique = pEffect->findTechnique(techName);

		if(!m_technique)
			base::lwrn<<"Technique \""<<techName<<"\" not found in effect \""<<pEffect->getName()<<"\".";
	}

	const PDynamicBinder& CMaterial::getDynamicBinder()
	{
		if(!m_pDynamicBinder)
			setEffect(TheRenderManager::Get().getDefaultEffect());
		return m_pDynamicBinder;
	}

	IEffect::ITechnique* CMaterial::getTechnique() const
	{
		return m_technique;
	}
}