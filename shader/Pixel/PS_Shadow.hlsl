struct PS_IN
{
    float4 pos : SV_Position;
    float4 Color : COLOR0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 shadowPos : TEXCOORD1;
};

SamplerState samw : register(s0);
SamplerState samp : register(s1);
Texture2D tex : register(t0);
Texture2D depthTex : register(t1);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samw, pin.uv);
    
    // 影で使うUVを計算
    float2 depthUV = pin.shadowPos.xy / pin.shadowPos.w;
    // 深度値を取得
    float pixelDepth = pin.shadowPos.z / pin.shadowPos.w;
    
    // UV座標を-1から1の範囲から0から1の範囲に変換しY座標の向きを反転
    depthUV = max(-1.0f, min(1.0f, depthUV)) * 0.5f + 0.5f;
    depthUV.y = -depthUV.y;
    
    // 深度テクスチャから深度の値を取得
    float depth = depthTex.Sample(samp, depthUV);
    
    // 取得した深度値から影の補正を行う
    color.rgb *= saturate(step(pixelDepth, depth) + 0.3f);
    
    return color;
}