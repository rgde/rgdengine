//----------------------------------------------------------------------------------------
//Textures, matrices and samplers.
//----------------------------------------------------------------------------------------

//Filters
#define LINEAR_FILTER MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR
#define ANIS04_FILTER MipFilter = LINEAR; MaxAnisotropy = 4; MinFilter = ANISOTROPIC; MagFilter = ANISOTROPIC
#define ANIS16_FILTER MipFilter = LINEAR; MaxAnisotropy =16; MinFilter = ANISOTROPIC; MagFilter = ANISOTROPIC

#define DECLARE_TEXTURE(name1, name2, name3)    \
shared Texture Texture##name1 : TEXTURE_##name2 \
<                                               \
	string mapname = name3;                     \
>;                                              \
                                                \
shared bool bEnabled##name1 : ENABLED_##name2   \
<                                               \
	string enabledflag = name3;                 \
> = false;                                      \
                                                \
shared float4x4 Matrix##name1 : MATRIX_##name2  \
<                                               \
	string matrixname = name3;                  \
>;                                              \
                                                \
sampler Sampler##name1 = sampler_state          \
{                                               \
	Texture = (Texture##name1);                 \
	LINEAR_FILTER;                              \
};

//Textures
//May contains height or bump scale in alpha chanel
DECLARE_TEXTURE(Bump, BUMP, "bump")
//May contains trasparency in alpha chanel
DECLARE_TEXTURE(Diffuse, DIFFUSE, "diffuse")
DECLARE_TEXTURE(Illumination, ILLUMINATION, "illumination")
DECLARE_TEXTURE(Lightmap, LIGHTMAP, "lightmap")
DECLARE_TEXTURE(Reflection, REFLECTION, "reflection")