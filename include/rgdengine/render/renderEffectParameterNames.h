//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 05.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: String names for effect parameters.
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace render
{
	//Material
	const std::string MaterialDiffuseColorParamName  = "MATERIALDIFFUSE";
	const std::string MaterialAmbientColorParamName  = "MATERIALAMBIENT";
	const std::string MaterialSpecularColorParamName = "MATERIALSPECULAR";
	const std::string MaterialEmissiveColorParamName = "MATERIALEMISSIVE";
	const std::string MaterialPowerValueParamName    = "MATERIALPOWER";

	const std::string TextureNamePrefix  = "TEXTURE_";
	const std::string TextureNamePostfix = "";

	//Matrices
	const std::string WorldViewProjectionMatrixParamName = "WORLDVIEWPROJ";
	const std::string WorldViewMatrixParamName           = "WORLDVIEW";
	const std::string WorldViewITMatrixParamName         = "WORLDVIEWIT";
	const std::string WorldMatrixParamName               = "WORLD";
	const std::string ViewMatrixParamName                = "VIEW";
	const std::string ViewITMatrixParamName              = "VIEWIT";
	const std::string ProjectionMatrixParamName          = "PROJECTION";

	//Pixel/vertex pipe parameters
	const std::string FillModeParamName = "FILL_MODE";
	const std::string FogParamName      = "FOG";

	//Lighting
	const std::string AmbientLightColorParamName = "AMBIENT_LIGHT";
}