struct PS_IN
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float4 NormalColor : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
    uint4 MaterialID : MATERIALID0;
};

struct PS_OUT
{
    float4 Normal : SV_Target0; // 法線をカラーで出力
    float4 Color : SV_Target1; // テクスチャを張り付けて出力
    float4 WorldPos : SV_Target2; // ポジションの出力
    float4 MaterialID : SV_Target3; // マテリアルIDの出力
};

struct MATERIAL
{
    float4 Albedo;
    float4 Emission;
    bool TextureEnable;
    float Metalic;
    float Roughness;
    float Shinness;
};
// マテリアル定数バッファ
cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
};

struct PBR_MAT
{
    float4 Aldbedo;
    float4 Emission;
    unsigned int MaterialID;
    float Metalic;
    float Roufhness;
    float MicroFacet;
    float Shiness;
    float NormalMap_Intensity;
    float2 pad;
};


cbuffer PBR_Buffer : register(b6)
{
    PBR_MAT PBR[1000];
}

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    // 法線をカラーで出力
    pout.Normal = pin.NormalColor;
    pout.Normal.a = 1.0f;
    // ポジションをカラーで出力(塗りつぶした場所はwを1.0fにする)
    pout.WorldPos = pin.WorldPos;
    pout.WorldPos.w = 1.0f;
    // マテリアルID
    pout.MaterialID = float4(1.0f, 1.0f, 1.0f, 1.0f);
    pout.MaterialID.r = pin.MaterialID.r / 65535.0f;
    
    // アルベド
    if (Material.TextureEnable)
    {
        pout.Color = g_Texture.Sample(g_SamplerState, pin.TexCoord);
        pout.Color *= pin.Color;
    }
    else
    {
        pout.Color = pin.Color;
        pout.Color.a = 1.0f;
    }
    
	return pout;
}