#include "precompiled.h"

#include <rgde/render/light_manager.h>
#include <rgde/base/xml_helpers.h>

namespace render
{
	//LightManager::SLightData::SLightData()
	//	:	m_nType(base_light::Point),
	//		m_range(0),
	//		m_attenuations(math::vec3f(1, 0, 0)),
	//		m_bEnable(false)
	//{
	//}

	//const math::Color& LightManager::getAmbientColor() const
	//{
	//	return m_ambientColor;
	//}

	//void LightManager::setAmbientColor(const math::Color& color)
	//{
	//	m_ambientColor = color;
	//}

	////bool LightManager::getDebugRendering() const
	////{
	////	return m_bRenderDebug;
	////}

	////void LightManager::setDebugRendering(bool bRenderDebug)
	////{
	////	if(bRenderDebug == m_bRenderDebug)
	////		return;

	////	m_bRenderDebug = bRenderDebug;

	////	for(ModelsList::iterator it = m_models.begin(); it != m_models.end(); it++)
	////		(*it)->visible(m_bRenderDebug);
	////}

	//int LightManager::getActivePointLightsIni() const
	//{
	//	return m_nActivePointLightsIni;
	//}
	//
	//int LightManager::getActivePointLightsNum() const
	//{
	//	return m_nActivePointLightsNum;
	//}

	//int LightManager::getPointLightsNum() const
	//{
	//	return m_nPointLightsNum;
	//}

	//int LightManager::getActiveSpotLightsIni() const
	//{
	//	return m_nActiveSpotLightsIni;
	//}
	//
	//int LightManager::getActiveSpotLightsNum() const
	//{
	//	return m_nActiveSpotLightsNum;
	//}

	//int LightManager::getSpotLightsNum() const
	//{
	//	return m_nSpotLightsNum;
	//}

	//int LightManager::getActiveDirectionalLightsIni() const
	//{
	//	return m_nActiveDirectionalLightsIni;
	//}
	//
	//int LightManager::getActiveDirectionalLightsNum() const
	//{
	//	return m_nActiveDirectionalLightsNum;
	//}

	//int LightManager::getDirectionalLightsNum() const
	//{
	//	return m_nDirectionalLightsNum;
	//}

	//int LightManager::getAllActiveLightsNum() const
	//{
	//	return m_nActiveDirectionalLightsNum + m_nActiveSpotLightsNum + m_nActivePointLightsNum;
	//}

	//int LightManager::getAllLightsNum() const
	//{
	//	return m_nDirectionalLightsNum + m_nSpotLightsNum + m_nPointLightsNum;
	//}

	//LightManager::LightManager()
	//	: 		m_bRenderDebug(false),
	//			m_bModelNamesReaded(false),
	//			m_nActivePointLightsIni(0),
	//			m_nActivePointLightsNum(0),
	//			m_nPointLightsNum(0),
	//			m_nActiveSpotLightsIni(0),
	//			m_nActiveSpotLightsNum(0),
	//			m_nSpotLightsNum(0),
	//			m_nActiveDirectionalLightsIni(0),
	//			m_nActiveDirectionalLightsNum(0),
	//			m_nDirectionalLightsNum(0)
	//{
	//}

	//void LightManager::addLight(const PLight& pLight)
	//{
	//	base_light::type tipe = pLight->type();

	//	switch(tipe)
	//	{
	//		case base_light::Point:
	//			m_nPointLightsNum++;
	//		break;

	//		case base_light::Spot:
	//			m_nSpotLightsNum++;
	//		break;

	//		case base_light::Directional:
	//			m_nDirectionalLightsNum++;
	//		break;
	//	};

	//	//std::string strLightModelName = lightTypeToStr(tipe);
	//	//model_ptr pModel = model::create(std::wstring(strLightModelName.begin(), strLightModelName.end()));
	//	//m_models.push_back(pModel);

	//	//pLight->getParentFrame()->add(pModel->getRoot());
	//	//pModel->visible(m_bRenderDebug);
	//	m_lights.push_back(pLight);
	//}
	//
	//void LightManager::removeLight(const PLight& pLight)
	//{
	//	Lights::iterator it1 = m_lights.begin();
	//	//ModelsList::iterator it2 = m_models.begin();
	//	//for(; it1 != m_lights.end() && it2 != m_models.end(); it1++, it2++)
	//	for(; it1 != m_lights.end(); it1++)
	//	{
	//		if(*it1 == pLight)
	//		{
	//			m_lights.erase(it1);
	//			//m_models.erase(it2);
	//			switch(pLight->type())
	//			{
	//				case base_light::Point:
	//					m_nPointLightsNum--;
	//				break;

	//				case base_light::Spot:
	//					m_nSpotLightsNum--;
	//				break;

	//				case base_light::Directional:
	//					m_nDirectionalLightsNum--;
	//				break;
	//			};
	//			return;
	//		}
	//	}
	//}

	//void LightManager::updateLightsDatas()
	//{
	//	m_nActivePointLightsIni = 0;
	//	m_nActivePointLightsNum = 0;
	//	m_nActiveSpotLightsIni = 0;
	//	m_nActiveSpotLightsNum = 0;
	//	m_nActiveDirectionalLightsIni = 0;
	//	m_nActiveDirectionalLightsNum = 0;

	//	Lights::const_iterator it;
	//	int i = 0;

	//	#define FILL_BASE_LIGHT_MEMBERS(light_data, pLight)   \
	//	light_data.m_nType     = pLight->type();           \
	//	light_data.m_position  = pLight->world_position(); \
	//	light_data.m_direction = pLight->at(); 	          \
	//	light_data.m_ambient   = pLight->getAmbient();        \
	//	light_data.m_diffuse   = pLight->getDiffuse();        \
	//	light_data.m_specular  = pLight->getSpecular();       \
	//	light_data.m_bEnable   = pLight->isEnabled();

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->type() == base_light::Point)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const PointLight* pPointLight =
	//								(const PointLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i], pPointLight)
	//					m_lightData[i].m_range = pPointLight->range();
	//					m_lightData[i].m_attenuations =
	//					math::vec3f(pPointLight->constant_attenuation(),
	//								pPointLight->linear_attenuation(),
	//								pPointLight->quadratic_attenuation());
	//					m_lightData[i].m_spot = math::vec3f(0, 0, 0);

	//					m_nActivePointLightsNum++;
	//					i++;
	//				}

	//	m_nActiveSpotLightsIni = i;

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->type() == base_light::Spot)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const SpotLight* pSpotLight =
	//								(const SpotLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i], pSpotLight)
	//					m_lightData[i].m_range = pSpotLight->range();
	//					m_lightData[i].m_attenuations =
	//					math::vec3f(pSpotLight->constant_attenuation(),
	//								pSpotLight->linear_attenuation(),
	//								pSpotLight->quadratic_attenuation());
	//					m_lightData[i].m_spot =
	//					math::vec3f(pSpotLight->getTheta(),
	//								pSpotLight->getPhi(),
	//								pSpotLight->getFalloff());

	//					m_nActiveSpotLightsNum++;
	//					i++;
	//				}

	//	m_nActiveDirectionalLightsIni = i;

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->type() == base_light::Directional)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const DirectionalLight* pDirectionalLight =
	//									(const DirectionalLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i],
	//													pDirectionalLight)
	//					m_lightData[i].m_range = 0;
	//					m_lightData[i].m_attenuations = math::vec3f(0, 0, 0);
	//					m_lightData[i].m_spot = math::vec3f(0, 0, 0);

	//					m_nActiveDirectionalLightsNum++;
	//					i++;
	//				}
	//}

	//void LightManager::readModelNames(const std::string &strConfigName)
	//{
	//	io::path_add_scoped("Common/");

	//	TiXmlDocument XmlConfig;
	//	if(!base::load_xml(strConfigName, XmlConfig))
	//	{
	//		base::lerr<<"LightManager::readModelsNames(): Can't open"<<
	//					" config file: "<<strConfigName;
	//		return;
	//	}

	//	TiXmlHandle hConfigHandle(&XmlConfig);
	//	TiXmlHandle hModelsNamesHandle =
	//				hConfigHandle.FirstChildElement("ModelsNames");

	//	m_strPointLightModelName = base::safe_read<std::string>(
	//								hModelsNamesHandle,
	//								"PointLight",
	//								"teapot.xml");
	//	m_strSpotLightModelName = base::safe_read<std::string>(
	//								hModelsNamesHandle,
	//								"SpotLight",
	//								"teapot.xml");
	//	m_strDirectionalLightModelName = base::safe_read<std::string>(
	//								hModelsNamesHandle,
	//								"DirectionalLight",
	//								"teapot.xml");

	//	m_bModelNamesReaded = true;
	//}

	//const std::string& LightManager::lightTypeToStr(base_light::type tipe)
	//{
	//	if(!m_bModelNamesReaded)
	//		readModelNames();

	//	switch(tipe)
	//	{
	//		case base_light::Point:
	//			return m_strPointLightModelName;
	//		break;

	//		case base_light::Spot:
	//			return m_strSpotLightModelName;
	//		break;

	//		case base_light::Directional:
	//			return m_strDirectionalLightModelName;
	//		break;
	//	};
	//}
}