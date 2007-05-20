//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 12.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////

#include "precompiled.h"
#include "render/renderBinders.h"
#include "render/renderEffectParameterNames.h"
#include "render/renderDevice.h"
#include "render/renderManager.h"
#include "render/renderLightManager.h"
#include "render/renderFog.h"
#include "base/lexical_cast.h"

namespace render
{
///---------------------------------------------------------------------------\
//|                              Dynamic Binder                               |
//\---------------------------------------------------------------------------/
	inline const math::PCamera& getCamera()
	{
		return TheDevice::Get().getCurentCamera();
	}

	math::Matrix44f makeWorldViewProjMatrix(const math::PFrame& frame)
	{
		const math::PCamera& pCamera = getCamera();
		const math::Matrix44f& mView = pCamera->getViewMatrix();
		const math::Matrix44f& mProj = pCamera->getProjMatrix();
		return mProj*(mView*frame->getFullTransform());
	}

	math::Matrix44f makeWorldViewMatrix(const math::PFrame& frame)
	{
		return frame->getFullTransform()*getCamera()->getViewMatrix();
	}

	math::Matrix44f makeWorldViewInvTranspMatrix(const math::PFrame& frame)
	{
		math::Matrix44f result = frame->getFullTransform();
		result *= getCamera()->getViewMatrix();
		math::invert(result);
		math::transpose(result);
		return result;
	}

	void addMatrixParameters(const PDynamicBinder& binder)
	{
		using namespace math;
		using namespace boost;
		typedef CDynamicBinder::Types<Matrix44f>::GetFunction
											GetMatrix44fFunction;

		GetMatrix44fFunction getMatrixFunction;
		
		getMatrixFunction = bind(&makeWorldViewProjMatrix, _1);
		binder->addParameter<Matrix44f>(getMatrixFunction,
										WorldViewProjectionMatrixParamName);
		getMatrixFunction = bind(&makeWorldViewMatrix, _1);
		binder->addParameter<Matrix44f>(getMatrixFunction,
										WorldViewMatrixParamName);
		getMatrixFunction = bind(&makeWorldViewInvTranspMatrix, _1);
		binder->addParameter<Matrix44f>(getMatrixFunction,
										WorldViewITMatrixParamName);
		getMatrixFunction = bind(&CFrame::getFullTransform, _1);
		binder->addParameter<Matrix44f>(getMatrixFunction,
										WorldMatrixParamName);
	}

	void addMaterialParameters(const PDynamicBinder& binder,
							   const CMaterial& mat)
	{
		using namespace math;
		using namespace boost;
		typedef CDynamicBinder::Types<math::Color>::ParamTypeGetFunction
													ParamTypeGetColorFunction;
		typedef CDynamicBinder::Types<float>::GetFunction GetFloatFunction;

		ParamTypeGetColorFunction getColorFunction;
		GetFloatFunction          getFloatFunction;

		getColorFunction = bind(&CMaterial::getDiffuseColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialDiffuseColorParamName);
		getColorFunction = bind(&CMaterial::getAmbientColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialAmbientColorParamName);
		getColorFunction = bind(&CMaterial::getSpecularColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialSpecularColorParamName);
		getColorFunction = bind(&CMaterial::getEmissiveColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialEmissiveColorParamName);
		getFloatFunction = bind(&CMaterial::getSpecularPower, mat);
		binder->addParameter<float>(getFloatFunction,
									MaterialPowerValueParamName);
	}

	std::string addTextureMaps(const PDynamicBinder& binder,
							   const CMaterial& mat)
	{
		const CMaterial::MaterialMaps& maps = mat.getMaterialMaps();
		std::string strTechName;

		CMaterial::MaterialMaps::const_iterator it;

		for(it = maps.begin(); it != maps.end(); it++)
			if(it->second.isTextureValid())
			{
				std::string parameterName = TextureNamePrefix +
											base::Upper_case(it->first) +
											TextureNamePostfix;

				if(binder->addParameter<PTexture>(it->second.getTexture(),
												  parameterName))
				{
					std::string lower = base::Lower_case(it->first);
					std::string upper = base::Upper_case(it->first);
					std::string temp = upper.substr(0, 1) +
									   lower.substr(1, lower.length() - 1);

					if(CubeTexture == it->second.getTexture()->getType())
						temp += "Cube";

					strTechName += temp;

					base::lmsg<<"Texture \""<<it->first<<
								"\" added to effect parameters.";
				}
				else
					base::lwrn<<"Texture \""<<it->first<<
								"\" not added to effect parameters.";
			}
			else
				base::lwrn<<"Texture \""<<it->first<<
							"\" is invalid. Not adding...";

		if(mat.isTransparent())
			strTechName += "Transparent";

		if("" == strTechName)
			strTechName = "Default";

		return strTechName;
	}

	PDynamicBinder createDynamicBinder(const PEffect& pEffect,
									   const CMaterial& mat,
									   std::string& techniqueName)
	{
		PDynamicBinder result = CDynamicBinder::Create(pEffect);

		if(result)
		{
			addMaterialParameters(result, mat);
			techniqueName = addTextureMaps(result, mat);
			addMatrixParameters(result);
		}

		return result;
	}


///---------------------------------------------------------------------------\
//|                              Static Binder                                |
//\---------------------------------------------------------------------------/
	void addPixelVertexPipeParameters(const PStaticBinder& binder)
	{
		using namespace math;
		using namespace boost;
		typedef CStaticBinder::Types<int>::GetFunction GetIntFunction;
		typedef CStaticBinder::Types<CFog>::ParamTypeGetFunction
												ParamTypeGetFogFunction;

		GetIntFunction          getIntFunction;
		ParamTypeGetFogFunction getFogFunction;

		getIntFunction = bind(&CRenderManager::getFillMode,
							  &TheRenderManager::Get());
		binder->addParameter<int>(getIntFunction,
								  FillModeParamName);
		getFogFunction = bind(&CRenderManager::getCurrentFog,
							  &TheRenderManager::Get());
		binder->addParameter<CFog>(getFogFunction,
								   FogParamName);
	}

	void addLightingParameters(const PStaticBinder& binder)
	{
		//using namespace math;
		//using namespace boost;
		//typedef CStaticBinder::Types<Color>::ParamTypeGetFunction
		//									ParamTypeGetColorFunction;
		//typedef CStaticBinder::Types<int>::GetFunction  GetIntFunction;
		//typedef CStaticBinder::Types<bool>::GetFunction GetBoolFunction;
		//typedef CLightManager::LightDatas LightDatas;
		//typedef CStaticBinder::Types<LightDatas>::GetFunction
		//											GetLightsFunction;

		//ParamTypeGetColorFunction getColorFunction;
		//GetIntFunction            getIntFunction;
		//GetBoolFunction           getBoolFunction;
		//GetLightsFunction         getLightsFunction;

		//getColorFunction = bind(&CLightManager::getAmbientColor,
		//						&TheLightManager::Get());
		//binder->addParameter<Color>(getColorFunction,
		//							AmbientLightColorParamName);

		//getIntFunction = bind(&CLightManager::getActivePointLightsIni,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTPOINTINI");

		//getIntFunction = bind(&CLightManager::getActivePointLightsNum,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTPOINTNUM");

		//getIntFunction = bind(&CLightManager::getActiveSpotLightsIni,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTSPOTINI");

		//getIntFunction = bind(&CLightManager::getActiveSpotLightsNum,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTSPOTNUM");

		//getIntFunction = bind(&CLightManager::getActiveDirectionalLightsIni,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTDIRINI");

		//getIntFunction = bind(&CLightManager::getActiveDirectionalLightsNum,
		//					  &TheLightManager::Get());
		//binder->addParameter<int>(getIntFunction, "LIGHTDIRNUM");

		//getBoolFunction = bind(&CRenderManager::isLightingEnabled,
		//					   &TheRenderManager::Get());
		//binder->addParameter<bool>(getBoolFunction, "LIGHTING_ENABLED");

		//getLightsFunction = bind(&CLightManager::getDatas,
		//						 &TheLightManager::Get());
		//binder->addParameter<LightDatas>(getLightsFunction, "LIGHTS");
	}

	math::Matrix44f getViewMatrix()
	{
		if(!getCamera())
			return math::Matrix44f();

		return getCamera()->getViewMatrix();
	}

	math::Matrix44f getProjMatrix()
	{
		if(!getCamera())
			return math::Matrix44f();

		return getCamera()->getProjMatrix();
	}

	math::Matrix44f makeViewInvTranspMatrix()
	{
		if(!getCamera())
			return math::Matrix44f();

		math::Matrix44f result = getCamera()->getViewMatrix();
		math::invert(result);
		math::transpose(result);
		return result;
	}

	void addMatrixParameters(const PStaticBinder& binder)
	{
		using namespace math;
		using namespace boost;
		typedef CStaticBinder::Types<Matrix44f>::GetFunction GetMatrix44fFunction;

		GetMatrix44fFunction getMatrixFunction;
		
		getMatrixFunction = bind(&getProjMatrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ProjectionMatrixParamName);

		getMatrixFunction = bind(&getViewMatrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ViewMatrixParamName);

		getMatrixFunction = bind(&makeViewInvTranspMatrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ViewITMatrixParamName);
	}

	PStaticBinder createStaticBinder(const PEffect& pEffect)
	{
		PStaticBinder binder = CStaticBinder::Create(pEffect);

		if(binder)
		{
			addPixelVertexPipeParameters(binder);
			addLightingParameters(binder);
			addMatrixParameters(binder);
		}

		return binder;
	}
}