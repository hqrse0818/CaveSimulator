struct PS_IN
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

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

SamplerState samp : register(s0);
Texture2D tex : register(t0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 OutColor = 0.0f;
    if(Material.Enable)
    {
        OutColor = tex.Sample(samp, pin.uv);
        OutColor *= (Material.Albedo * pin.color);
    }
    else
    {
        OutColor = (Material.Albedo * pin.color);
    }
    
	return OutColor;
}