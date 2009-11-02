//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 12.07.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: 
// example of use:
//////////////////////////////////////////////////////////////////////////
#include "precompiled.h"

#include <rgde/render/binders.h>
#include <rgde/render/effect_parameter_names.h>
#include <rgde/render/device.h>
#include <rgde/render/manager.h>
#include <rgde/render/light_manager.h>
#include <rgde/render/fog.h>

#include <rgde/base/lexical_cast.h>

namespace render
{
///---------------------------------------------------------------------------\
//|                              Dynamic binder                               |
//\---------------------------------------------------------------------------/
	inline const math::camera_ptr& get_camera()
	{
		return TheDevice::get().get_camera();
	}

	math::Matrix44f makeWorldViewProjMatrix(const math::frame_ptr& frame)
	{
		const math::camera_ptr& camera = get_camera();
		const math::Matrix44f& mView = camera->get_view_matrix();
		const math::Matrix44f& mProj = camera->get_proj_matrix();
		return mProj*(mView*frame->get_full_tm());
	}

	math::Matrix44f makeWorldViewMatrix(const math::frame_ptr& frame)
	{
		return frame->get_full_tm()*get_camera()->get_view_matrix();
	}

	math::Matrix44f makeWorldViewInvTranspMatrix(const math::frame_ptr& frame)
	{
		math::Matrix44f result = frame->get_full_tm();
		result *= get_camera()->get_view_matrix();
		math::invert(result);
		math::transpose(result);
		return result;
	}

	void addMatrixParameters(const dynamic_binder_ptr& binder)
	{
		using namespace math;
		using namespace boost;
		typedef dymamic_binder_ptr::Types<Matrix44f>::getter
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
		getMatrixFunction = bind(&frame::get_full_tm, _1);
		binder->addParameter<Matrix44f>(getMatrixFunction,
										WorldMatrixParamName);
	}

	void addMaterialParameters(const dynamic_binder_ptr& binder,
							   const material& mat)
	{
		using namespace math;
		using namespace boost;
		typedef dymamic_binder_ptr::Types<math::Color>::ParamTypeGetFunction
													ParamTypeGetColorFunction;
		typedef dymamic_binder_ptr::Types<float>::getter GetFloatFunction;

		ParamTypeGetColorFunction getColorFunction;
		GetFloatFunction          getFloatFunction;

		getColorFunction = bind(&material::getDiffuseColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialDiffuseColorParamName);
		getColorFunction = bind(&material::getAmbientColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialAmbientColorParamName);
		getColorFunction = bind(&material::getSpecularColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialSpecularColorParamName);
		getColorFunction = bind(&material::getEmissiveColor, mat);
		binder->addParameter<Color>(getColorFunction,
									MaterialEmissiveColorParamName);
		getFloatFunction = bind(&material::getSpecularPower, mat);
		binder->addParameter<float>(getFloatFunction,
									MaterialPowerValueParamName);
	}

	std::string addTextureMaps(const dynamic_binder_ptr& binder,
							   const material& mat)
	{
		const material::MaterialMaps& maps = mat.getMaterialMaps();
		std::string strTechName;

		material::MaterialMaps::const_iterator it;

		for(it = maps.begin(); it != maps.end(); it++)
			if(it->second.isTextureValid())
			{
				std::string parameterName = TextureNamePrefix +
											base::upper_case(it->first) +
											TextureNamePostfix;

				if(binder->addParameter<texture_ptr>(it->second.get_texture(),
												  parameterName))
				{
					std::string lower = base::lower_case(it->first);
					std::string upper = base::upper_case(it->first);
					std::string temp = upper.substr(0, 1) +
									   lower.substr(1, lower.length() - 1);

					if(CubeTexture == it->second.get_texture()->get_type())
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

	dynamic_binder_ptr createDynamicBinder(const effect_ptr& pEffect,
									   const material& mat,
									   std::string& techniqueName)
	{
		dynamic_binder_ptr result = dymamic_binder_ptr::create(pEffect);

		if(result)
		{
			addMaterialParameters(result, mat);
			techniqueName = addTextureMaps(result, mat);
			addMatrixParameters(result);
		}

		return result;
	}


///---------------------------------------------------------------------------\
//|                              Static binder                                |
//\---------------------------------------------------------------------------/
	void addPixelVertexPipeParameters(const PStaticBinder& binder)
	{
		using namespace math;
		using namespace boost;
		typedef StaticBinder::Types<int>::getter GetIntFunction;
		typedef StaticBinder::Types<Fog>::ParamTypeGetFunction
												ParamTypeGetFogFunction;

		GetIntFunction          getIntFunction;
		ParamTypeGetFogFunction getFogFunction;

		getIntFunction = bind(&render_manager::getFillMode,
							  &TheRenderManager::get());
		binder->addParameter<int>(getIntFunction,
								  FillModeParamName);
		getFogFunction = bind(&render_manager::getCurrentFog,
							  &TheRenderManager::get());
		binder->addParameter<Fog>(getFogFunction,
								   FogParamName);
	}

	void addLightingParameters(const PStaticBinder& binder)
	{
		//using namespace math;
		//using namespace boost;
		//typedef StaticBinder::Types<Color>::ParamTypeGetFunction
		//									ParamTypeGetColorFunction;
		//typedef StaticBinder::Types<int>::getter  GetIntFunction;
		//typedef StaticBinder::Types<bool>::getter GetBoolFunction;
		//typedef LightManager::LightDatas LightDatas;
		//typedef StaticBinder::Types<LightDatas>::getter
		//											GetLightsFunction;

		//ParamTypeGetColorFunction getColorFunction;
		//GetIntFunction            getIntFunction;
		//GetBoolFunction           getBoolFunction;
		//GetLightsFunction         getLightsFunction;

		//getColorFunction = bind(&LightManager::getAmbientColor,
		//						&TheLightManager::get());
		//binder->addParameter<Color>(getColorFunction,
		//							AmbientLightColorParamName);

		//getIntFunction = bind(&LightManager::getActivePointLightsIni,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTPOINTINI");

		//getIntFunction = bind(&LightManager::getActivePointLightsNum,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTPOINTNUM");

		//getIntFunction = bind(&LightManager::getActiveSpotLightsIni,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTSPOTINI");

		//getIntFunction = bind(&LightManager::getActiveSpotLightsNum,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTSPOTNUM");

		//getIntFunction = bind(&LightManager::getActiveDirectionalLightsIni,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTDIRINI");

		//getIntFunction = bind(&LightManager::getActiveDirectionalLightsNum,
		//					  &TheLightManager::get());
		//binder->addParameter<int>(getIntFunction, "LIGHTDIRNUM");

		//getBoolFunction = bind(&render_manager::isLightingEnabled,
		//					   &TheRenderManager::get());
		//binder->addParameter<bool>(getBoolFunction, "LIGHTING_ENABLED");

		//getLightsFunction = bind(&LightManager::getDatas,
		//						 &TheLightManager::get());
		//binder->addParameter<LightDatas>(getLightsFunction, "LIGHTS");
	}

	math::Matrix44f get_view_matrix()
	{
		if(!get_camera())
			return math::Matrix44f();

		return get_camera()->get_view_matrix();
	}

	math::Matrix44f get_proj_matrix()
	{
		if(!get_camera())
			return math::Matrix44f();

		return get_camera()->get_proj_matrix();
	}

	math::Matrix44f makeViewInvTranspMatrix()
	{
		if(!get_camera())
			return math::Matrix44f();

		math::Matrix44f result = get_camera()->get_view_matrix();
		math::invert(result);
		math::transpose(result);
		return result;
	}

	void addMatrixParameters(const PStaticBinder& binder)
	{
		using namespace math;
		using namespace boost;
		typedef StaticBinder::Types<Matrix44f>::getter GetMatrix44fFunction;

		GetMatrix44fFunction getMatrixFunction;
		
		getMatrixFunction = bind(&get_proj_matrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ProjectionMatrixParamName);

		getMatrixFunction = bind(&get_view_matrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ViewMatrixParamName);

		getMatrixFunction = bind(&makeViewInvTranspMatrix);
		binder->addParameter<Matrix44f>(getMatrixFunction, ViewITMatrixParamName);
	}

	PStaticBinder createStaticBinder(const effect_ptr& pEffect)
	{
		PStaticBinder binder = StaticBinder::create(pEffect);

		if(binder)
		{
			addPixelVertexPipeParameters(binder);
			addLightingParameters(binder);
			addMatrixParameters(binder);
		}

		return binder;
	}
}