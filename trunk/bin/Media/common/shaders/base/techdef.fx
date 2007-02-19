//----------------------------------------------------------------------------------------
//Macroses to define techniques
//----------------------------------------------------------------------------------------

#define SETUP_BASE_STATES            \
/*FOG*/                              \
SETUP_FOG                            \
                                     \
/*RENDER STATES*/                    \
FillMode = (FillMode);               \
NormalizeNormals = true;			 \
                                     \
/*TRANSFORMATION MATRICES*/          \
ViewTransform       = (View);        \
ProjectionTransform = (Projection);  \
WorldTransform[0]   = (World);       \
                                     \
/*MATERIAL*/                         \
SETUP_MATERIAL                       \
                                     \
/*ANY STATES*/                       \
SpecularEnable = true;               \
ZEnable	       = true;               \
ColorVertex    = false;               \
                                     \
/*LIGHTING*/                         \
Lighting = (LightingEnabled);        \
Ambient  = (vAmbientLight);          \
SETUP_LIGHTS

#define BEGIN_TECHNIQUE(name, sm) \
technique name                    \
<                                 \
	string ShaderModel = sm;      \
>                                 \
{                                 \
    pass pass0                    \
    {                             \
        SETUP_BASE_STATES


#define END_TECHNIQUE_WITHOUT_BLENDING \
        ZWriteEnable = true;           \
                                       \
        CullMode = NONE;               \
                                       \
        NO_BLENDING                    \
    }                                  \
}


#define END_TECHNIQUE_WITH_BLENDING \
        ZWriteEnable = false;       \
                                    \
        CullMode = CW;              \
                                    \
        ALPHA_BLENDING              \
    }                               \
    pass pass1                      \
    {                               \
        CullMode = CCW;             \
    }                               \
}