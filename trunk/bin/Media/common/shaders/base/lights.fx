//----------------------------------------------------------------------------------------
//Lights.
//----------------------------------------------------------------------------------------

#define NUM_LIGHTS               8

#define LIGHT_TYPE_NONE          0
#define LIGHT_TYPE_POINT         1
#define LIGHT_TYPE_SPOT          2
#define LIGHT_TYPE_DIRECTIONAL   3
#define LIGHT_NUM_TYPES          4

shared bool	LightingEnabled : LIGHTING_ENABLED;
shared float4	vAmbientLight   : AMBIENT_LIGHT;

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
shared int iLightPointIni : LIGHTPOINTINI = 0;
shared int iLightPointNum : LIGHTPOINTNUM = 1;
shared int iLightSpotIni : LIGHTSPOTINI = 0;
shared int iLightSpotNum : LIGHTSPOTNUM = 0;
shared int iLightDirIni : LIGHTDIRINI = 0;
shared int iLightDirNum : LIGHTDIRNUM = 0;

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


shared CLight lights[NUM_LIGHTS] : LIGHTS = {                //NUM_LIGHTS == 8
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT,
   DEFAULT_LIGHT
};

#define SETUP_LIGHT(n)                             \
LightPosition[n]     = (lights[n].vPos);           \
LightDirection[n]    = (lights[n].vDir);           \
LightDiffuse[n]      = (lights[n].vDiffuse);       \
LightSpecular[n]     = (lights[n].vSpecular);      \
LightAmbient[n]      = (lights[n].vAmbient);       \
LightEnable[n]       = (lights[n].bEnabled);       \
LightRange[n]        = (lights[n].fRange);         \
LightFalloff[n]      = (lights[n].vSpot.z);        \
LightTheta[n]        = (lights[n].vSpot.x);        \
LightPhi[n]          = (lights[n].vSpot.y);        \
LightAttenuation0[n] = (lights[n].vAttenuation.x); \
LightAttenuation1[n] = (lights[n].vAttenuation.y); \
LightAttenuation2[n] = (lights[n].vAttenuation.z); \
LightType[n]         = (lights[n].iType);
		
#define SETUP_LIGHTS \
SETUP_LIGHT(0)       \
SETUP_LIGHT(1)       \
SETUP_LIGHT(2)       \
SETUP_LIGHT(3)       \
SETUP_LIGHT(4)       \
SETUP_LIGHT(5)       \
SETUP_LIGHT(6)       \
SETUP_LIGHT(7)

float4 CalcPosition(float3 pos)
{
	float4 v = mul(float4(pos, 1), WorldViewProj);
	return v;
}

float3 CalcViewSpacePosition(float3 vPos)
{
	return mul(float4(vPos, 1), (float4x3)WorldView);
}

float3 CalcViewSpaceNormal(float3 vNorm)
{
	return normalize(mul(vNorm, (float3x3)WorldView));
}

//bool bSpecular = true;

//-----------------------------------------------------------------------------
// Name: DoDirLight()
// Desc: Directional light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoDirLight(float3 N, float3 V, int i)
{
   COLOR_PAIR Out;
   float3 L = mul((float3x3)ViewIT, -normalize(lights[i].vDir));
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   if(NdotL > 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      //if(bSpecular)
      {
         float3 H = normalize(L + V);   //half vector
         Out.ColorSpec = pow(max(0, dot(H, N)), fPower) * lights[i].vSpecular;
      }
   }
   return Out;
}

//-----------------------------------------------------------------------------
// Name: DoPointLight()
// Desc: Point light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoPointLight(float4 vPosition, float3 N, float3 V, int i)
{
   float3 L = mul((float3x3)ViewIT, normalize((lights[i].vPos-(float3)mul(World,vPosition))));
   COLOR_PAIR Out;
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   float fAtten = 1.f;
   if(NdotL >= 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      //if(bSpecular)
      //{
         float3 H = normalize(L + V);   //half vector
         Out.ColorSpec = pow(max(0, dot(H, N)), fPower) * lights[i].vSpecular;
      //}

      float LD = length(lights[i].vPos-(float3)mul(World,vPosition));
      if(LD > lights[i].fRange)
      {
         fAtten = 0.f;
      }
      else
      {
         fAtten *= 1.f/(lights[i].vAttenuation.x + lights[i].vAttenuation.y*LD + lights[i].vAttenuation.z*LD*LD);
      }
      Out.Color *= fAtten;
      Out.ColorSpec *= fAtten;
   }
   return Out;
}

//-----------------------------------------------------------------------------
// Name: DoSpotLight()
// Desc: Spot light computation
//-----------------------------------------------------------------------------
COLOR_PAIR DoSpotLight(float4 vPosition, float3 N, float3 V, int i)
{
   float3 L = mul((float3x3)ViewIT, normalize((lights[i].vPos-(float3)mul(World,vPosition))));
   COLOR_PAIR Out;
   float NdotL = dot(N, L);
   Out.Color = lights[i].vAmbient;
   Out.ColorSpec = 0;
   float fAttenSpot = 1.f;
   if(NdotL >= 0.f)
   {
      //compute diffuse color
      Out.Color += NdotL * lights[i].vDiffuse;

      //add specular component
      //if(bSpecular)
      //{
      //   float3 H = normalize(L + V);   //half vector
      //   Out.ColorSpec = pow(max(0, dot(H, N)), fPower) * lights[i].vSpecular;
      //}

      float LD = length(lights[i].vPos-(float3)mul(World,vPosition));
      if(LD > lights[i].fRange)
      {
         fAttenSpot = 0.f;
      }
      else
      {
         fAttenSpot *= 1.f/(lights[i].vAttenuation.x + lights[i].vAttenuation.y*LD + lights[i].vAttenuation.z*LD*LD);
      }

      //spot cone computation
      float3 L2 = mul((float3x3)ViewIT, -normalize(lights[i].vDir));
      float rho = dot(L, L2);
      fAttenSpot *= pow(saturate((rho - lights[i].vSpot.y)/(lights[i].vSpot.x - lights[i].vSpot.y)), lights[i].vSpot.z);

      Out.Color *= fAttenSpot;
      Out.ColorSpec *= fAttenSpot;
   }
   return Out;
}