#include "precompiled.h"

#include <rgde/render/renderLightManager.h>
#include <rgde/base/xml_helpers.h>

namespace render
{
	//CLightManager::SLightData::SLightData()
	//	:	m_nType(ILight::Point),
	//		m_fRange(0),
	//		m_attenuations(math::Vec3f(1, 0, 0)),
	//		m_bEnable(false)
	//{
	//}

	//const math::Color& CLightManager::getAmbientColor() const
	//{
	//	return m_ambientColor;
	//}

	//void CLightManager::setAmbientColor(const math::Color& color)
	//{
	//	m_ambientColor = color;
	//}

	////bool CLightManager::getDebugRendering() const
	////{
	////	return m_bRenderDebug;
	////}

	////void CLightManager::setDebugRendering(bool bRenderDebug)
	////{
	////	if(bRenderDebug == m_bRenderDebug)
	////		return;

	////	m_bRenderDebug = bRenderDebug;

	////	for(ModelsList::iterator it = m_models.begin(); it != m_models.end(); it++)
	////		(*it)->setVisible(m_bRenderDebug);
	////}

	//int CLightManager::getActivePointLightsIni() const
	//{
	//	return m_nActivePointLightsIni;
	//}
	//
	//int CLightManager::getActivePointLightsNum() const
	//{
	//	return m_nActivePointLightsNum;
	//}

	//int CLightManager::getPointLightsNum() const
	//{
	//	return m_nPointLightsNum;
	//}

	//int CLightManager::getActiveSpotLightsIni() const
	//{
	//	return m_nActiveSpotLightsIni;
	//}
	//
	//int CLightManager::getActiveSpotLightsNum() const
	//{
	//	return m_nActiveSpotLightsNum;
	//}

	//int CLightManager::getSpotLightsNum() const
	//{
	//	return m_nSpotLightsNum;
	//}

	//int CLightManager::getActiveDirectionalLightsIni() const
	//{
	//	return m_nActiveDirectionalLightsIni;
	//}
	//
	//int CLightManager::getActiveDirectionalLightsNum() const
	//{
	//	return m_nActiveDirectionalLightsNum;
	//}

	//int CLightManager::getDirectionalLightsNum() const
	//{
	//	return m_nDirectionalLightsNum;
	//}

	//int CLightManager::getAllActiveLightsNum() const
	//{
	//	return m_nActiveDirectionalLightsNum + m_nActiveSpotLightsNum + m_nActivePointLightsNum;
	//}

	//int CLightManager::getAllLightsNum() const
	//{
	//	return m_nDirectionalLightsNum + m_nSpotLightsNum + m_nPointLightsNum;
	//}

	//CLightManager::CLightManager()
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

	//void CLightManager::addLight(const PLight& pLight)
	//{
	//	ILight::EType eType = pLight->getType();

	//	switch(eType)
	//	{
	//		case ILight::Point:
	//			m_nPointLightsNum++;
	//		break;

	//		case ILight::Spot:
	//			m_nSpotLightsNum++;
	//		break;

	//		case ILight::Directional:
	//			m_nDirectionalLightsNum++;
	//		break;
	//	};

	//	//std::string strLightModelName = lightTypeToStr(eType);
	//	//PModel pModel = Model::Create(std::wstring(strLightModelName.begin(), strLightModelName.end()));
	//	//m_models.push_back(pModel);

	//	//pLight->getParentFrame()->addChild(pModel->getRoot());
	//	//pModel->setVisible(m_bRenderDebug);
	//	m_lights.push_back(pLight);
	//}
	//
	//void CLightManager::removeLight(const PLight& pLight)
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
	//			switch(pLight->getType())
	//			{
	//				case ILight::Point:
	//					m_nPointLightsNum--;
	//				break;

	//				case ILight::Spot:
	//					m_nSpotLightsNum--;
	//				break;

	//				case ILight::Directional:
	//					m_nDirectionalLightsNum--;
	//				break;
	//			};
	//			return;
	//		}
	//	}
	//}

	//void CLightManager::updateLightsDatas()
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
	//	light_data.m_nType     = pLight->getType();           \
	//	light_data.m_position  = pLight->getGlobalPosition(); \
	//	light_data.m_direction = pLight->getAt(); 	          \
	//	light_data.m_ambient   = pLight->getAmbient();        \
	//	light_data.m_diffuse   = pLight->getDiffuse();        \
	//	light_data.m_specular  = pLight->getSpecular();       \
	//	light_data.m_bEnable   = pLight->isEnabled();

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->getType() == ILight::Point)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const CPointLight* pPointLight =
	//								(const CPointLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i], pPointLight)
	//					m_lightData[i].m_fRange = pPointLight->getRange();
	//					m_lightData[i].m_attenuations =
	//					math::Vec3f(pPointLight->getConstantAttenuation(),
	//								pPointLight->getLinearAttenuation(),
	//								pPointLight->getQuadraticAttenuation());
	//					m_lightData[i].m_spot = math::Vec3f(0, 0, 0);

	//					m_nActivePointLightsNum++;
	//					i++;
	//				}

	//	m_nActiveSpotLightsIni = i;

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->getType() == ILight::Spot)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const CSpotLight* pSpotLight =
	//								(const CSpotLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i], pSpotLight)
	//					m_lightData[i].m_fRange = pSpotLight->getRange();
	//					m_lightData[i].m_attenuations =
	//					math::Vec3f(pSpotLight->getConstantAttenuation(),
	//								pSpotLight->getLinearAttenuation(),
	//								pSpotLight->getQuadraticAttenuation());
	//					m_lightData[i].m_spot =
	//					math::Vec3f(pSpotLight->getTheta(),
	//								pSpotLight->getPhi(),
	//								pSpotLight->getFalloff());

	//					m_nActiveSpotLightsNum++;
	//					i++;
	//				}

	//	m_nActiveDirectionalLightsIni = i;

	//	for(it = m_lights.begin(); it != m_lights.end(); it++)
	//		if(*it)
	//			if((*it)->isEnabled())
	//				if((*it)->getType() == ILight::Directional)
	//				{
	//					if(i >= MAX_NUM_ACTIVE_LIGHTS)
	//						return;

	//					const CDirectionalLight* pDirectionalLight =
	//									(const CDirectionalLight*)it->get();

	//					FILL_BASE_LIGHT_MEMBERS(m_lightData[i],
	//													pDirectionalLight)
	//					m_lightData[i].m_fRange = 0;
	//					m_lightData[i].m_attenuations = math::Vec3f(0, 0, 0);
	//					m_lightData[i].m_spot = math::Vec3f(0, 0, 0);

	//					m_nActiveDirectionalLightsNum++;
	//					i++;
	//				}
	//}

	//void CLightManager::readModelNames(const std::string &strConfigName)
	//{
	//	io::ScopePathAdd("Common/");

	//	TiXmlDocument XmlConfig;
	//	if(!base::loadXml(strConfigName, XmlConfig))
	//	{
	//		base::lerr<<"CLightManager::readModelsNames(): Can't open"<<
	//					" config file: "<<strConfigName;
	//		return;
	//	}

	//	TiXmlHandle hConfigHandle(&XmlConfig);
	//	TiXmlHandle hModelsNamesHandle =
	//				hConfigHandle.FirstChildElement("ModelsNames");

	//	m_strPointLightModelName = base::safeReadValue<std::string>(
	//								hModelsNamesHandle,
	//								"PointLight",
	//								"teapot.xml");
	//	m_strSpotLightModelName = base::safeReadValue<std::string>(
	//								hModelsNamesHandle,
	//								"SpotLight",
	//								"teapot.xml");
	//	m_strDirectionalLightModelName = base::safeReadValue<std::string>(
	//								hModelsNamesHandle,
	//								"DirectionalLight",
	//								"teapot.xml");

	//	m_bModelNamesReaded = true;
	//}

	//const std::string& CLightManager::lightTypeToStr(ILight::EType eType)
	//{
	//	if(!m_bModelNamesReaded)
	//		readModelNames();

	//	switch(eType)
	//	{
	//		case ILight::Point:
	//			return m_strPointLightModelName;
	//		break;

	//		case ILight::Spot:
	//			return m_strSpotLightModelName;
	//		break;

	//		case ILight::Directional:
	//			return m_strDirectionalLightModelName;
	//		break;
	//	};
	//}
}