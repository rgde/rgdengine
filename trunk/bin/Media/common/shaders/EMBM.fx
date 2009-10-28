//----------------------------------------------------------------------------------------
//Environment Mapped Bump Mapping
//----------------------------------------------------------------------------------------
/*
struct VS_INPUT
{
	float4 position  : POSITION;
	float4 diffuse   : COLOR0;
	float3 specular  : COLOR1;
	float2 texCoord0 : TEXCOORD0;//Diffuse, Self-Illumination and Bump
	float2 texCoord1 : TEXCOORD1;//Lightmap
	float3 normal    : NORMAL;
};

struct VS_OUTPUT
{
	float4 position  : POSITION;
	float4 diffuse   : COLOR0;
	float3 specular  : COLOR1;
	float2 texCoord0 : TEXCOORD0;//Diffuse, Self-Illumination and Bump
	float2 texCoord1 : TEXCOORD1;//Lightmap
	float3 texCoord2 : TEXCOORD2;//Reflection
};

VS_OUTPUT EMBM_VS(VS_INPUT input, uniform bool bCubeMap)
{ 
    VS_OUTPUT output;

	output.position  = mul(input.position, World);
	output.diffuse   = input.diffuse;
	output.specular  = input.specular;
	output.texCoord0 = input.texCoord0;
	output.texCoord1 = input.texCoord1;

	if(bCubeMap)
	{
		//TODO:
	}
	else
	{
		float3 cameraSpaceNormal = mul(float4(input.normal, 1.0f), WorldView);

		output.texCoord2 = float3(cameraSpaceNormal.x/2.0f + 0.5f,
								  cameraSpaceNormal.y/2.0f + 0.5f,
								  0.0f);
	}

	return output;
}
*/
struct PS_INPUT
{
	float4 diffuse   : COLOR0;
	float4 specular  : COLOR1;
	float2 texCoord0 : TEXCOORD0;//Diffuse, Self-Illumination and Bump
	float2 texCoord1 : TEXCOORD1;//Lightmap
	float3 texCoord2 : TEXCOORD2;//Reflection
};

struct PS_OUTPUT
{
	float4 diffuse : COLOR0;
};

PS_OUTPUT EMBM(PS_INPUT input, uniform bool bCubeMap)
{ 
    PS_OUTPUT output;

    output.diffuse = input.diffuse;

	if(bEnabledDiffuse)
	{
		float4 diffuseTexel = tex2D(SamplerDiffuse, input.texCoord0);
	    output.diffuse *= diffuseTexel;
    }

	if(bEnabledLightmap)
	{
		float4 lightmapTexel = tex2D(SamplerLightmap, input.texCoord1);
		output.diffuse.rgb *= lightmapTexel.rgb;
	}

    output.diffuse.rgb += input.specular.rgb;

	if(bEnabledIllumination)
	{
		float4 illuminationTexel = tex2D(SamplerIllumination, input.texCoord0);
	    output.diffuse.rgb += illuminationTexel.rgb;
	}

	float4 bumpTexel = tex2D(SamplerBump, input.texCoord0);
	float4 reflectionTexel = { 0.0f, 0.0f, 0.0f, 0.0f };

	float3 bumpNormal = bumpTexel.xyz*2.0f - float3(1.0f, 1.0f, 1.0f);

	if(bCubeMap)
	{
		//TODO:
	}
	else
	{
		float2 reflectionTexCoords = input.texCoord2.xy + 
									bumpNormal.xz*bumpTexel.w;

		reflectionTexel = tex2D(SamplerReflection, reflectionTexCoords);
	}

	output.diffuse.rgb += reflectionTexel.rgb;

	return output;
}