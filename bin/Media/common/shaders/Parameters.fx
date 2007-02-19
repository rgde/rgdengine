#ifndef PARAMETERS_INCLUDE
#define PARAMETERS_INCLUDE

//----------------------------------------------------------------------------------------
//Fog.
//----------------------------------------------------------------------------------------

struct CFog
{
   bool  bEnabled;
   int   color;
   int   iMode;
   float fStart;
   float fEnd;
};

shared CFog fog : FOG = { false, 0, 0, 0.0, 0.0 };

//----------------------------------------------------------------------------------------
//Lights.
//----------------------------------------------------------------------------------------

#define NUM_LIGHTS               8

#define LIGHT_TYPE_NONE          0
#define LIGHT_TYPE_POINT         1
#define LIGHT_TYPE_SPOT          2
#define LIGHT_TYPE_DIRECTIONAL   3
#define LIGHT_NUM_TYPES          4

shared bool	  LightingEnabled : LIGHTING_ENABLED;
shared float4 vAmbientLight   : AMBIENT_LIGHT;

struct COLOR_PAIR
{
   float4 Color         : COLOR0;
   float4 ColorSpec     : COLOR1;
};

struct CLight
{
   int iType;
   float3 vPos;
   float3 vDir;
   float4 vAmbient;
   float4 vDiffuse;
   float4 vSpecular;
   float  fRange;
   float3 vAttenuation; //1, D, D^2;
   float3 vSpot;
   bool   bEnabled;
};

//initial and range of directional, point and spot lights within the light array
int iLightPointIni : LIGHTPOINTINI = 0;
int iLightPointNum : LIGHTPOINTNUM = 1;
int iLightSpotIni : LIGHTSPOTINI = 0;
int iLightSpotNum : LIGHTSPOTNUM = 0;
int iLightDirIni : LIGHTDIRINI = 0;
int iLightDirNum : LIGHTDIRNUM = 0;

#define DEFAULT_LIGHT              \
{                                  \
   LIGHT_TYPE_POINT,               \
   float3(0.0f, 0.0f, 0.0f),       \
   float3(0.0f, 0.0f, 0.0f),       \
   float4(0.0f, 0.0f, 0.0f, 0.0f), \
   float4(0.0f, 0.0f, 0.0f, 0.0f), \
   float4(0.0f, 0.0f, 0.0f, 0.0f), \
   0.0f,                           \
   float3(1.f, 0.f, 0.f),          \
   float3(0.f, 0.f, 0.f),          \
   false                           \
}


CLight lights[NUM_LIGHTS] : LIGHTS = {                //NUM_LIGHTS == 8
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT
};


//----------------------------------------------------------------------------------------
//Material.
//----------------------------------------------------------------------------------------

float4  vDiffuseColor   : MATERIALDIFFUSE;
float4  vAmbientColor   : MATERIALAMBIENT;
float4  vSpecularColor  : MATERIALSPECULAR;
float4  vEmissiveColor  : MATERIALEMISSIVE;
float   fPower          : MATERIALPOWER;

//----------------------------------------------------------------------------------------
//Matrices.
//----------------------------------------------------------------------------------------

float4x4 WorldViewProj : WORLDVIEWPROJ;
float4x4 WorldView     : WORLDVIEW;
float4x4 World         : WORLD;
float4x4 WorldViewIT   : WORLDVIEWIT;
shared float4x4 ViewIT        : VIEWIT;
shared float4x4 View          : VIEW;
shared float4x4 Projection    : PROJECTION;

//----------------------------------------------------------------------------------------
//Render states.
//----------------------------------------------------------------------------------------

shared int FillMode : FILL_MODE;

//----------------------------------------------------------------------------------------
//Textures and samplers.
//----------------------------------------------------------------------------------------

//Filters
#define LINEAR_FILTER MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR
#define ANIS04_FILTER MipFilter = LINEAR; MaxAnisotropy = 4; MinFilter = ANISOTROPIC; MagFilter = ANISOTROPIC
#define ANIS16_FILTER MipFilter = LINEAR; MaxAnisotropy =16; MinFilter = ANISOTROPIC; MagFilter = ANISOTROPIC

#define DECLARE_TEXTURE(name1, name2, name3)    \
Texture Texture##name1 : TEXTURE_##name2        \
<                                               \
	string mapname = name3;                     \
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

#endif