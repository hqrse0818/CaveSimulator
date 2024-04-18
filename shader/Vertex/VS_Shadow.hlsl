// 入力
struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

// 出力
struct PS_IN
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 ShadowPos : TEXCOORD1;
};

struct MATRIXBUFFER
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Scale;
};
// 行列類の定数バッファ
cbuffer Matrix0 : register(b0)
{
    MATRIXBUFFER matbuf;
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
    float4 LBaseColor;
    int Num;
    float3 LDummy;
};

PS_IN main(VS_IN vin)
{
    PS_IN pout;
    // スケールバッファと計算
    vin.Pos.xyz *= matbuf.Scale.xyz;
    
    // ライト用法線情報計算
    float4 worldNormal, normal;
    normal = float4(vin.Normal.xyz, 0.0);
    worldNormal = mul(normal, matbuf.World);
    worldNormal = normalize(worldNormal);
    
   // 法線
    float4 nor = float4(vin.Normal.xyz, 0.0f);
    pout.Normal = normalize(mul(nor, matbuf.World));
    
    // UV座標の設定
    pout.TexCoord = vin.TexCoord;
    pout.Color = vin.Diffuse;
    
    // 座標変換
    matrix wvp;
    wvp = mul(matbuf.World, matbuf.View);
    wvp = mul(wvp, matbuf.Projection);
    pout.Pos = mul(vin.Pos, wvp);
    
    // ライト視点の座標
    matrix lwvp;
    vin.Pos = mul(vin.Pos, matbuf.World);
    lwvp = mul(matbuf.World, LView);
    lwvp = mul(lwvp, LProj);
    pout.ShadowPos = mul(vin.Pos, lwvp);
    
    return pout;
}