//#include "base\shared.fx"

#define DISABLE_FOG  \
FogEnable = false;   \
FogTableMode = None;

float4x4 g_mLVP;


struct VS_OUTPUT
{
	float4 pos: POSITION;
	float4 color: COLOR0;
};


VS_OUTPUT Lines3dVS(float4 posIn: POSITION, float4 colorIn: COLOR0)
{
	VS_OUTPUT output;

	output.pos = mul(posIn, g_mLVP);
	output.color = colorIn;

	return output;
}

technique Lines2d
{
	pass p0
	{
		DISABLE_FOG

		ZEnable	        = true;
		Lighting        = false;
		ZWriteEnable    = false;
		FillMode        = WIREFRAME;
		CullMode        = NONE;
			
		// enable alpha blending
		AlphaBlendEnable = TRUE;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;


        ColorOp[0]	= SelectArg1;
        ColorArg1[0]	= Diffuse;

        AlphaOp[0]	= SelectArg1;
        AlphaArg1[0]	= Diffuse;

        ColorOp[1]	= Disable;
        AlphaOp[1]	= Disable;
	}
}


technique Lines3d
{
	pass p0
	{
		DISABLE_FOG

		ZEnable	        = true;
		Lighting        = false;
		ZWriteEnable    = false;
		FillMode        = WIREFRAME;
		CullMode        = NONE;
			
		// enable alpha blending
		AlphaBlendEnable = TRUE;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		    	    
        ColorOp[0]	= SelectArg1;
        ColorArg1[0]	= Diffuse;

        AlphaOp[0]	= SelectArg1;
        AlphaArg1[0]	= Diffuse;

        ColorOp[1]	= Disable;
        AlphaOp[1]	= Disable;


		VertexShader = compile vs_1_1 Lines3dVS();
	}
}