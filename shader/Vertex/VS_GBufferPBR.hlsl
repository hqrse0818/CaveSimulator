struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    uint4 MaterialID : MATERIALID0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float4 NormalColor : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
    uint4 MaterialID : MATERIALID0;
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
    float4 AlbedoColor;
    float4 Emission;
    bool TextureEnable;
    float Metallic;
    float Roughness;
    float Shinness;
};
// マテリアル定数バッファ
cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
}


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    // スケールバッファと計算
    vin.Pos.xyz *= matbuf.Scale.xyz;
    
    // 法線
    float4 nor = float4(vin.Normal.xyz, 0.0f);
    vout.NormalColor = normalize(mul(nor, matbuf.World));
    
    // UV座標
    vout.TexCoord = vin.TexCoord;
    vout.Color = vin.Diffuse;
    vout.MaterialID = vin.MaterialID[0];
    
     // ワールド座標の出力
    vout.WorldPos = mul(matbuf.World, vin.Pos);
    vout.WorldPos = mul(vin.Pos, matbuf.World);
    
    // 座標変換
    matrix wvp;
    wvp = mul(matbuf.World, matbuf.View);
    wvp = mul(wvp, matbuf.Projection);
    vout.Pos = mul(vin.Pos, wvp);
    
	return vout;
}