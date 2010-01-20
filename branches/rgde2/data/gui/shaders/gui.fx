texture guiTexture;
sampler2D guiSampler: register(s0) = sampler_state {
    Texture = <guiTexture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

float2 ViewPortSize;

struct appData 
{
    float4 Position	: POSITION;			
    float4 Color	: COLOR;
    //float2 Uv		: TEXCOORD0;
};

struct vertexOutData 
{
    float4 HPosition : POSITION;
    float4 Color	 : COLOR;
    float2 Uv		 : TEXCOORD0;
}; 

//------------------------------------
vertexOutData GuiVS(appData IN) 
{
    vertexOutData OUT;
  	        
    OUT.HPosition = IN.Position;
    
    OUT.HPosition.x = (2.0 * IN.Position.x / ViewPortSize.x) - 1.0;
    OUT.HPosition.y = 1.0 - (2.0 * IN.Position.y / ViewPortSize.y);
    OUT.HPosition.z = 1.0;
    OUT.HPosition.w = 1.0;
    
    
    
    OUT.Uv.xy = IN.Position.zw;
    OUT.Color = IN.Color;
       	   	    
    return OUT;
}

float4 GuiPS(vertexOutData IN) : COLOR 
{
  return tex2D(guiSampler, IN.Uv) * IN.Color;
    
}

//-----------------------------------
technique Simple
{
    pass p0 
    {	
    	ZEnable = false;
    	ZWriteEnable = false;
    	
    	CullMode = None;
    
    	AlphaBlendEnable = true;
    	SrcBlend = SrcAlpha;
	    DestBlend = InvSrcAlpha;
	    
	    
	    
		VertexShader = compile vs_2_0 GuiVS();
		PixelShader = compile ps_2_0 GuiPS();
    }
    
}