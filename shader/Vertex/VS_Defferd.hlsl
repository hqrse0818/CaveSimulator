struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 pos : SV_POSITION0;
    float2 uv : TEXCOORD0;
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

PS_OUT main(VS_IN vin)
{
    PS_OUT pout;
    
    pout.pos.xyz = vin.Pos.xyz * matbuf.Scale.xyz;
    
    matrix wvp;
    wvp = mul(matbuf.World, matbuf.View);
    wvp = mul(wvp, matbuf.Projection);
    
    pout.pos = mul(vin.Pos, wvp);
    pout.uv = vin.TexCoord;
    
	return pout;
}