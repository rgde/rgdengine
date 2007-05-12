#include "base\shared.fx"

texture SpriteTexture;
sampler SpriteSampler = sampler_state
{
    Texture = <SpriteTexture>;
    MinFilter = Linear;  
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

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

        //Texture[0]		= <spriteTexture>;
        Sampler[0]		= <SpriteSampler>;
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

        //Texture[0]		= <spriteTexture>;
        Sampler[0]		= <SpriteSampler>;
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