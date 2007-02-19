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

#define SETUP_FOG           \
FogEnable = (fog.bEnabled); \
FogColor = (fog.color);     \
FogTableMode = (fog.iMode); \
FogStart = (fog.fStart);    \
FogEnd = (fog.fEnd);

#define DISABLE_FOG  \
FogEnable = false;   \
FogTableMode = None;