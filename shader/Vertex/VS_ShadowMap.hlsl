// 入力
struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : SV_Position0;
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

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    vin.Pos.xyz *= matbuf.Scale.xyz;
    
    vout.pos = mul(vin.Pos, matbuf.World);
    vout.pos = mul(vout.pos, LView);
    vout.pos = mul(vout.pos, LProj);
    
	return vout;
}