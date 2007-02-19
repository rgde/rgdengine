//----------------------------------------------------------------------------------------
//Materials.
//----------------------------------------------------------------------------------------

//MATERIAL
shared float4  vDiffuseColor   : MATERIALDIFFUSE;
shared float4  vAmbientColor   : MATERIALAMBIENT;
shared float4  vSpecularColor  : MATERIALSPECULAR;
shared float4  vEmissiveColor  : MATERIALEMISSIVE;
shared float   fPower          : MATERIALPOWER;

#define SETUP_MATERIAL               \
MaterialDiffuse  = (vDiffuseColor);  \
MaterialAmbient  = (vAmbientColor);  \
MaterialEmissive = (vEmissiveColor); \
MaterialSpecular = (vSpecularColor); \
MaterialPower    = (fPower);