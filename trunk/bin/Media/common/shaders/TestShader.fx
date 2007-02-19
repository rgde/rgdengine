#include "base\\shared.fx"

//-----------------------------------------------------------
// Vertex Shader code goes here
//-----------------------------------------------------------

float4x4 WorldViewMatrix;

float4 GetDirLightColor(in float3 N, in float3 V)
{
    float4 color=0;
    float3 lightDir = normalize(g_directionalLight.m_vDirection);
    lightDir = mul(lightDir, (float3x3)g_mViewMatrix);
    float diffuse_factor = max(0, dot(-lightDir, N));
        
    // Calculate a half vector for specular light component
    float3 vhalf = normalize(V+lightDir);
    //float3 vhalf = normalize(normalize(mul(float3(0,0,1), g_mViewMatrix))+lightDir);
    //float3 vhalf = normalize(float3(0,0,1) + lightDir);
        
    //Calculate specular light factor
    float specular_factor = pow(max(0, dot(N, -vhalf)), g_fMaterialPower);
        
        
    color = //g_vMaterialAmbientColor * g_directionalLight.m_vAmbient + // Ambient component
            //g_vMaterialDiffuseColor * g_directionalLight.m_vDiffuse * diffuse_factor + //Diffuse component
            g_vMaterialSpecularColor * g_directionalLight.m_vSpecular * specular_factor + // Specular component
            g_vMaterialEmissiveColor; // Emissive component
    return color;
}

struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 color    : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 normal   : NORMAL;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    WorldViewMatrix = mul(g_mWorldMatrix,g_mViewMatrix);
    float4x4 WVPMatrix = mul(WorldViewMatrix, g_mProjectionMatrix);
    
    output.position = mul(input.position, WVPMatrix);
    
    // Calculate normal for light calculation
    float3 N = mul((float3)input.normal, (float3x3)g_mWorldViewITMatrix);
    
    // Calculate viewer position for light calculation
    float3 V = normalize(mul(input.position, WorldViewMatrix));
    
    output.color = GetDirLightColor(N, V);
    output.texcoord = input.texcoord; 
    return output;
}

//-----------------------------------------------------------
// Effect body
//-----------------------------------------------------------
technique DefaultTechnique
{
    pass pass0
    {
    VertexShader = compile vs_1_1 VS();
    
    //CAMERA MATRICES
    ViewTransform       = (g_mViewMatrix);
    ProjectionTransform = (g_mProjectionMatrix);

    //RENDER STATES
//        Lighting = (g_bLightingEnabled);
        Lighting = false;
    FillMode = (g_nFillMode);
    Ambient  = (g_vAmbientColor);

    //WORLD MATRIX
    WorldTransform[0] = (g_mWorldMatrix);

    //MATERIAL
    Texture[0]     = (g_tMaterialDiffuseTexture);
    MaterialDiffuse  = (g_vMaterialDiffuseColor);
    MaterialAmbient  = (g_vMaterialAmbientColor);
    MaterialSpecular = (g_vMaterialSpecularColor);
    MaterialEmissive = (g_vMaterialEmissiveColor);
    MaterialPower    = (g_fMaterialPower);

    //POINT LIGHT
    LightPosition[0]     = (g_pointLight.m_vPosition);
    LightDiffuse[0]      = (g_pointLight.m_vDiffuse);
    LightSpecular[0]     = (g_pointLight.m_vSpecular);
    LightAmbient[0]      = (g_pointLight.m_vAmbient);
    LightEnable[0]       = (g_pointLight.m_bEnable);
    LightRange[0]        = (g_pointLight.m_fRange);
    LightAttenuation0[0] = 1.0;//(g_pointLight.m_fAttenuation0);
    LightAttenuation1[0] = (g_pointLight.m_fAttenuation1);
    LightAttenuation2[0] = (g_pointLight.m_fAttenuation2);
    LightType[0]         = POINT;


    //SPOT LIGHT
    LightPosition[1]     = (g_spotLight.m_vPosition);
    LightDirection[1]    = {0, 0, 1};//(g_spotLight.m_vDirection);
    LightDiffuse[1]      = (g_spotLight.m_vDiffuse);
    LightSpecular[1]     = (g_spotLight.m_vSpecular);
    LightAmbient[1]      = (g_spotLight.m_vAmbient);
    LightEnable[1]       = (g_spotLight.m_bEnable);
    LightRange[1]        = (g_spotLight.m_fRange);
    LightFalloff[1]      = (g_spotLight.m_fFalloff);
    LightTheta[1]        = (g_spotLight.m_fTheta);
    LightPhi[1]          = (g_spotLight.m_fPhi);
    LightAttenuation0[1] = 1.0;//(g_spotLight.m_fAttenuation0);
    LightAttenuation1[1] = (g_spotLight.m_fAttenuation1);
    LightAttenuation2[1] = (g_spotLight.m_fAttenuation2);
    LightType[1]         = SPOT;


    //DIRECTIONAL LIGHT
    LightDirection[2]    = {0, 0, 1};//(g_directionalLight.m_vDirection);
    LightDiffuse[2]      = (g_directionalLight.m_vDiffuse);
    LightSpecular[2]     = (g_directionalLight.m_vSpecular);
    LightAmbient[2]      = (g_directionalLight.m_vAmbient);
    LightEnable[2]       = (g_directionalLight.m_bEnable);
    LightType[2]         = DIRECTIONAL;

    //ANY STATES
    SpecularEnable = true;
    ZWriteEnable   = true;

    // Disable alpha blending
    AlphaBlendEnable = false;

    // Set up texture stage 0
    ColorOp[0]   = Modulate;
    ColorArg1[0] = Texture;
    ColorArg2[0] = Diffuse;
    AlphaOp[0]   = Modulate;
    AlphaArg1[0] = Texture;
    AlphaArg2[0] = Diffuse;
    MinFilter[0] = Linear;
    MagFilter[0] = Linear;
    MipFilter[0] = Linear;

        // Disable texture stage 1
        ColorOp[1] = Disable;
        AlphaOp[1] = Disable;
    }
}