struct PS_IN    
{
    float4 pos : SV_Position;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t1);
SamplerState samp : register(s0);

// RGBカラーから輝度計算を行う
float Luminance(float3 _color)
{
    return _color.r * 0.299f + _color.g * 0.587f + _color.b * 0.114f;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    
    // ピクセル周辺でサンプリングするためのUV座標の差分の計算(画面サイズ)
    float2 diff = float2(0.5f / 1280.0f, 0.5f / 720.0f);
    // 斜め方向の周辺のピクセルの色の輝度を計算
    float lumiLT = Luminance(tex.Sample(samp, pin.uv + diff * float2(-1.0f, 1.0f)).rgb);
    float lumiRT = Luminance(tex.Sample(samp, pin.uv + diff * float2(1.0f, 1.0f)).rgb);
    float lumiLB = Luminance(tex.Sample(samp, pin.uv + diff * float2(-1.0f, -1.0f)).rgb);
    float lumiRB = Luminance(tex.Sample(samp, pin.uv + diff * float2(1.0f, -1.0f)).rgb);
    float lumiC = Luminance(color.rgb);
    
    // 周辺の輝度の最大値と最小値を求める
    float lumiMax = max(max(lumiLT, lumiRT), max(lumiLB, lumiRB));
    float lumiMin = min(min(lumiLT, lumiRT), min(lumiLB, lumiRB));
    // 最大輝度と最小輝度の差を計算
    float lumiSub = max(lumiMax, lumiC) - min(lumiMin, lumiC);
    
    // 輝度の差が一定値未満ならば、エイリアスを修正する必要が無いと判断し、元の色を出力
    //if (lumiSub < 0.125f)
    if (lumiSub < 0.05)
    {
        return color;
    }
    
    // エッジの方向を計算
    float dirRT = lumiRT - lumiLB;
    float dirRB = lumiRB - lumiLT;
    float2 dir = float2(dirRT + dirRB, dirRT - dirRB);
    
    // エッジの方向を正規化
    dir = normalize(dir) * diff;
    
    // エッジの方向に沿って色をサンプリング
    float4 col1 = tex.Sample(samp, pin.uv + dir);
    float4 col2 = tex.Sample(samp, pin.uv - dir);
    
    // サンプリングした色をブレンド
    return (col1 + col2) * 0.5f;
}