//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 14.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: LightManager manages lights.
// example of use:
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <rgde/render/light.h>

namespace render
{
	//const int MAX_NUM_ACTIVE_LIGHTS = 8;
	//const int MAX_NUM_LIGHTS = 8;

	//class LightManager
	//{
	//friend class ILight;
	//public:

	//	typedef std::list<PLight> Lights;

	//public:

	//	const math::Color& getAmbientColor() const;
	//	void               setAmbientColor(const math::Color& color);


	//	inline const Lights& getLights() const
	//	{
	//		return m_lights;
	//	}

	//	//bool getDebugRendering() const;
	//	//void setDebugRendering(bool bRenderDebug);

	//	//ACTIVE are enabled lights
	//	int getActivePointLightsIni() const;
	//	int getActivePointLightsNum() const;
	//	int getPointLightsNum() const;

	//	int getActiveSpotLightsIni() const;
	//	int getActiveSpotLightsNum() const;
	//	int getSpotLightsNum() const;

	//	int getActiveDirectionalLightsIni() const;
	//	int getActiveDirectionalLightsNum() const;
	//	int getDirectionalLightsNum() const;

	//	int getAllActiveLightsNum() const;
	//	int getAllLightsNum() const;

	//	struct SLightData
	//	{
	//		SLightData();

	//		int         m_nType;
	//		math::Vec3f m_position;
	//		math::Vec3f m_direction;
	//		math::Vec4f m_ambient;
	//		math::Vec4f m_diffuse;
	//		math::Vec4f m_specular;
	//		float       m_fRange;
	//		math::Vec3f m_attenuations;//1, D, D^2;
	//		math::Vec3f m_spot;
	//		bool        m_bEnable;
	//	};

	//	typedef std::vector<SLightData> LightDatas;

	//	inline LightDatas getDatas()
	//	{
	//		updateLightsDatas();
	//		LightDatas lights;
	//		for(uint i = 0; i < MAX_NUM_ACTIVE_LIGHTS; i++)
	//			lights.push_back(m_lightData[i]);
	//		return lights;
	//	}

	//protected:
	//	LightManager();

	//private:
	//	void addLight(const PLight& pLight);
	//	void removeLight(const PLight& pLight);

	//	void updateLightsDatas();

	//	void readModelNames(const std::string& strConfigName =
	//										"LightsModelsNames.xml");
	//	const std::string& lightTypeToStr(ILight::EType eType);

	//private:
	//	math::Color m_ambientColor;
	//	Lights      m_lights;

	//	bool m_bRenderDebug;

	//	//typedef std::list<PModel> Models;
	//	//Models m_models;

	//	bool m_bModelNamesReaded;

	//	std::string m_strPointLightModelName;
	//	std::string m_strSpotLightModelName;
	//	std::string m_strDirectionalLightModelName;

	//	SLightData m_lightData[MAX_NUM_ACTIVE_LIGHTS];

	//	int m_nActivePointLightsIni;
	//	int m_nActivePointLightsNum;
	//	int m_nPointLightsNum;
	//	int m_nActiveSpotLightsIni;
	//	int m_nActiveSpotLightsNum;
	//	int m_nSpotLightsNum;
	//	int m_nActiveDirectionalLightsIni;
	//	int m_nActiveDirectionalLightsNum;
	//	int m_nDirectionalLightsNum;
	//};

	//typedef base::TSingelton<LightManager> TheLightManager;
}