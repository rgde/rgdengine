//#include "base\shared.fx"

#define DISABLE_FOG  \
FogEnable = false;   \
FogTableMode = None;

texture spriteTexture;

technique aditive
{
   pass p0
   {
		DISABLE_FOG

		Lighting        = false;
		ZWriteEnable    = false;

        AlphaBlendEnable= TRUE;
        SrcBlend        = SrcAlpha;
        DestBlend       = One;

        Texture[0]		= <spriteTexture>;
        ColorOp[0]		= Modulate;
        ColorArg1[0]	= Texture;
        ColorArg2[0]	= Diffuse;

        AlphaOp[0]		= Modulate;
        AlphaArg1[0]	= Texture;
        AlphaArg2[0]	= Diffuse;

        MinFilter[0]	= Linear;
        MagFilter[0]	= Linear;
        MipFilter[0]	= Linear;

        ColorOp[1]		= Disable;
        AlphaOp[1]		= Disable;
   }
}

technique alpha
{
   pass p0
   {
		DISABLE_FOG

		Lighting        = false;
		ZWriteEnable    = false;

        AlphaBlendEnable= TRUE;
        SrcBlend        = SrcAlpha;
        DestBlend       = INVSRCALPHA;

        Texture[0]		= <spriteTexture>;
        ColorOp[0]		= Modulate;
        ColorArg1[0]	= Texture;
        ColorArg2[0]	= Diffuse;

        AlphaOp[0]		= Modulate;
        AlphaArg1[0]	= Texture;
        AlphaArg2[0]	= Diffuse;

        MinFilter[0]	= Linear;
        MagFilter[0]	= Linear;
        MipFilter[0]	= Linear;

        ColorOp[1]		= Disable;
        AlphaOp[1]		= Disable;
   }
}