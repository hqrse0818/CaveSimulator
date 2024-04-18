struct VS_IN
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct MATRIXBUFFER
{
    matrix Wolrd;
    matrix View;
    matrix Proj;
};

cbuffer Matrix0 : register(b0)
{
    MATRIXBUFFER matbuf;
}

struct MATERIAL
{
    float4 Albedo;
    float4 Emission;
    bool Enable;
    float Metalic;
    float Roughness;
    float pad;
};

cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    matrix wvp;
    wvp = mul(matbuf.Wolrd, matbuf.View);
    wvp = mul(wvp, matbuf.Proj);
    
    vout.pos = mul(vin.pos, wvp);
    vout.color = vin.color;
    vout.uv = vin.uv;
    
	return vout;
}