// 入力
struct PS_IN
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float4 NormalColor : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
};

// 出力
struct PS_OUT
{
    float4 Normal : SV_Target0; // 法線をカラーで出力
    float4 Color : SV_Target1; // テクスチャを張り付けて出力
    float4 WorldPos : SV_Target2; // ポジションの出力
    float4 ScreenPos : SV_Target3;  //  スクリーン上ポジション
};

struct MATRIXBUFFER
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Scale;
    float4 CameraPos;
};
// 行列類の定数バッファ
cbuffer Matrix0 : register(b0)
{
    MATRIXBUFFER matbuf;
}

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float Roughness;
    float Dummy;
};
// マテリアル定数バッファ
cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
}

cbuffer DirectionLight : register(b2)
{
    bool LightEnable;
    float3 LightPos;
    float4 LightDirection;
    float4 LightColor;
    float4 LightAmbient;
    matrix LWorld;
    matrix LView;
    matrix LProj;
    float4 EyePos;
    float4 LBaseColor;
    int Num;
    int SpotLightNum;
    float2 LDummy;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    // 法線をカラーで出力
    pout.Normal = (pin.NormalColor / 2.0f) + 0.5f;
    pout.Normal.a = 1.0f;
    // 法線のaの値にスペキュラ強度を保存
    //pout.Normal.a = Material.Shininess;
    // ポジションをカラーで出力(塗りつぶした場所はwを1.0fにする)
    pout.WorldPos = pin.WorldPos;
    pout.WorldPos.w = 1.0f;
    // スペキュラカラー
    //pout.Specular = Material.Specular;
    // エミッション
    //pout.Emissin = Material.Emission;
    //pout.Emissin.a = Material.Roughness;
    
    // スクリーン上のポジション
    pout.ScreenPos = pin.Pos;
    
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