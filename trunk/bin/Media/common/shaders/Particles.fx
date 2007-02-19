#include "base\shared.fx"

float4x4 m_mLVP;
float4   m_vRight;
float4   m_vUp;

texture ParticlesTexture;	// particles texture


struct VS_OUTPUT{
	float4 Pos: POSITION;
	float2 Tex: TEXCOORD0;
	float4 Col: COLOR0;
};

VS_OUTPUT ParticlesVS(  float4 InPos: POSITION,
						float2 InTex: TEXCOORD0,
						float2 InOff: TEXCOORD1,
						float4 InCol: COLOR0)
{
	VS_OUTPUT OutStruct;

	float4 Position = (InOff.x * m_vRight) + (InOff.y * m_vUp) + InPos;

	OutStruct.Pos = mul(Position,m_mLVP);
	OutStruct.Col = InCol;
	OutStruct.Tex = InTex;

	return OutStruct;
}

technique PartilesRenderModulate
{
    pass p0
    {
	Lighting	 = false;
	ZWriteEnable	 = false;
	CullMode = CCW;

//	ADDITIVE_BLENDING2
	ALPHA_BLENDING
	SETUP_FOG
//	DISABLE_FOG

/*        
        // enable alpha blending
        AlphaBlendEnable = TRUE;
        SrcBlend         = SrcAlpha;
        DestBlend        = InvSrcAlpha;
*/        
        // Set up texture stage 0
        Texture[0]	= <ParticlesTexture>; // Use the texture parameter defined above
        ColorOp[0]	= Modulate;
        ColorArg1[0]	= Texture;
        ColorArg2[0]	= Diffuse;
        
        AlphaOp[0]	= Modulate;
        AlphaArg1[0]	= Texture;
        AlphaArg2[0]	= Diffuse;
        
        MinFilter[0]	= Linear;
        MagFilter[0]	= Linear;
        MipFilter[0]	= Linear;
        
        // Disable texture stage 1
        ColorOp[1]	= Disable;
        AlphaOp[1]	= Disable;
        
        VertexShader = compile vs_1_1 ParticlesVS();
   }
}