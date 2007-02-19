//----------------------------------------------------------------------------------------
//Macroses to setup stages
//----------------------------------------------------------------------------------------


//Color
#define SETUP_COLOR_STAGE1(n, operation, arg1) \
ColorOp[n]     = operation;                    \
ColorArg1[n]   = arg1;

#define SETUP_COLOR_STAGE2(n, operation, arg1, arg2) \
ColorOp[n]     = operation;                         \
ColorArg1[n]   = arg1;                              \
ColorArg2[n]   = arg2;

#define SETUP_COLOR_STAGE3(n, operation, arg1, arg2, arg3) \
ColorOp[n]     = operation;                                \
ColorArg1[n]   = arg1;                                     \
ColorArg2[n]   = arg2;                                     \
ColorArg0[n]   = arg3; 

#define DISABLE_COLOR_STAGE(n) \
ColorOp[n] = DISABLE;


//Alpha
#define SETUP_ALPHA_STAGE1(n, operation, arg1) \
AlphaOp[n]     = operation;                    \
AlphaArg1[n]   = arg1;

#define SETUP_ALPHA_STAGE2(n, operation, arg1, arg2) \
AlphaOp[n]     = operation;                          \
AlphaArg1[n]   = arg1;                               \
AlphaArg2[n]   = arg2;

#define SETUP_ALPHA_STAGE3(n, operation, arg1, arg2, arg3) \
AlphaOp[n]     = operation;                                \
AlphaArg1[n]   = arg1;                                     \
AlphaArg2[n]   = arg2;                                     \
AlphaArg0[n]   = arg3;

#define DISABLE_ALPHA_STAGE(n) \
AlphaOp[n] = DISABLE;

//Textures
#define GEN_NO PASSTHRU
#define GEN_CUBE_MAP CAMERASPACEREFLECTIONVECTOR
#define GEN_SPHERE_MAP SPHEREMAP

#define SETUP_TEXTURE(m, name1, name2, gen, adress_mode) \
Sampler[m] = (name1);                                    \
TexCoordIndex[m] = gen;                                  \
MipFilter[m] = LINEAR;                                   \
MinFilter[m] = Linear;                                   \
MagFilter[m] = LINEAR;                                   \
AddressU[m] = adress_mode;                               \
AddressV[m] = adress_mode;                               \
AddressW[m] = adress_mode;

//TextureTransform[m] = (name2);                           \
//TextureTransformFlags[m] = count3;                       \