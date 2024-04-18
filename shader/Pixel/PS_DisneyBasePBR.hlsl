struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 Diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};

struct PS_OUT
{
    float4 Result : SV_Target0;
    float4 LightMap : SV_Target1;
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

static const float PI = 3.1415926f;
static const int MAXPointLightNum = 1024;

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

// ディレクショナルライト定数バッファ
cbuffer DirectionLight : register(b2)
{
    bool LightEnable;
    float3 LightPos;
    float4 LightDirection;
    float3 LightDiffuse;
    float LightPower;
    float4 LightAmbient;
    matrix LWorld;
    matrix LView;
    matrix LProj;
    float4 LBaseColor;
    int PointLightNum;
    int SpotLightNum;
    float2 LDummy;
};



// ポイントライト定義
struct PointLight
{
    float3 pLightPos;
    float pRange;
    float3 pColor;
    float pPower;
};

// ポイントライト定数バッファ
cbuffer PointLights : register(b4)
{
    PointLight pLights[MAXPointLightNum];
}

struct SpotLight
{
    float4 sColor;
    float3 sLightPos;
    float sRange;
    float3 sDirection;
    float sAngle;
};

cbuffer SpotLights : register(b5)
{
    SpotLight sLights[1000];
}

cbuffer PBR_Buffer : register(b6)
{
    PBR_MAT Materials[1000];
}

Texture2D NormalTex : register(t1);
Texture2D AlbedoTex : register(t2);
Texture2D WorldPosTex : register(t3);
Texture2D MaterialTex : register(t4);
TextureCube CubeTex : register(t5);
SamplerState samp : register(s0);
SamplerState samp1 : register(s2);

// ベックマン分布の計算
float Beckmann(float m, float t);
// GGX関数
float GGX(float3 _LDir, float3 _toEye, float3 _Normal, float _Roughness);
// フレネルの計算
float SpcFrescel(float f, float u);
// フレネル反射を考慮した拡散反射光を求める
float CalcDiffuseFromFresnel(float3 _normal, float3 _ldir, float3 _toEye, float _Roughness);
// ランバート拡散反射光を求める
float3 CalcLambertDiffuse(float3 _lightDirection, float3 _lightColor, float3 _normal);
// Cook-Trranceモデルの鏡面反射計算
float CookTrranceSpecular(float3 _LDir, float3 _toEye, float3 _normal, float _metallic, float _microfacet, float _smooth);
// セル調計算
float CalcCellDiffuse(float3 _normal, float3 _ldir);

float4 main(PS_IN pin) : SV_TARGET
{
    // 法線
    float3 Normal = NormalTex.Sample(samp, pin.uv).xyz;
    float3 N = normalize(Normal);
    // ワールド座標
    float3 World = WorldPosTex.Sample(samp, pin.uv).xyz;
    // アルベドカラー
    float4 AlbedoColor = AlbedoTex.Sample(samp, pin.uv);
    // スペキュラカラー
    float3 SpecularColor = AlbedoColor.rgb;
    
    // マテリアルID
    uint MatID = uint(MaterialTex.Sample(samp, pin.uv).r * 65535.0f);
    
    // ベース色
    float3 BaseColor = Materials[MatID].Aldbedo.rgb;
    // 金属度
    float Metallic = Materials[MatID].Metalic;
    // 滑らかさ
    float Roufness = Materials[MatID].Roufhness;
    float Smooth = 1.0f - Roufness;
    
    // 微小表面
    float MicroFacet = Materials[MatID].MicroFacet;
    // スペキュラ強度
    float SpecularPower = Materials[MatID].Shiness;
    
    // 視線に向かって伸びるベクトル
    float3 toEyeVec = normalize(matbuf.CameraPos.xyz - World);
    
    // 反射ベクトル
    //float3 ReflectDir = reflect(-toEyeVec, Normal);
    //float4 ReflectColor = CubeTex.SampleLevel(samp, ReflectDir, Roufness * 5) * Metallic;
    //float4 ReflectColor = CubeTex.Sample(samp1, ReflectDir) * Metallic;
    
    // スペキュラ強度真面目計算?
    //float3 HalfLV = normalize(toEyeVec + LightDirection.xyz);
    //float toEyeTheta = pow(acos(dot(N, HalfLV)), SpecularPower);
    //toEyeTheta = dot(LightPower, toEyeTheta);
    //SpecularPower = toEyeTheta;
    // ここまで
    
    float3 lightColor = 0;
    float3 NlightColor = 0;
    
    // ----------------------- //
    // 平行光源のライティング //
    // ----------------------- //
    float DiffuseFromFresnel = CalcDiffuseFromFresnel(N, -LightDirection.xyz, toEyeVec, Roufness);
    
    // 正規化したLambert拡散反射光を計算
    float3 LambertDiffuse = CalcLambertDiffuse(-LightDirection.xyz, LightDiffuse, N);
    
    // 最終的な拡散反射光
    float3 diffuse = AlbedoColor.rgb * DiffuseFromFresnel * LambertDiffuse;
    
    // Cook-Trranceモデルを利用した鏡面反射率を計算
    float CTSpecular = CookTrranceSpecular(-LightDirection.xyz, toEyeVec, N, Metallic, MicroFacet, Smooth);
    float3 LightSpecular = LightDiffuse * CTSpecular * SpecularPower;
    
    // 金属度が高ければ、鏡面反射はスペキュラカラー低ければ白に近づく
    LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
    // 滑らかさを使って拡散反射光と鏡面反射光を合成する(滑らかさが高いほど拡散反射は弱くなる)
    lightColor += diffuse * Roufness + LightSpecular;
    //NlightColor += AlbedoColor.rgb * (CalcCellDiffuse(N, -LightDirection.xyz) + LightAmbient.rgb);
    
    // ---------------------------- //
    // ポイントライトのライティング //
    // ---------------------------- //
    for (int i = 0; i < PointLightNum; i++)
    {
        // ポイントライト
        PointLight PLight = pLights[i];
        // ワールド座標からポイントライトに向かうベクトル
        float3 toPointLightDirection = PLight.pLightPos.xyz - World;
        float3 L = normalize(toPointLightDirection);
        // 長さ
        float Length = length(toPointLightDirection);
        
        // スペキュラ強度真面目計算?
        //float3 HalfLV = normalize(toEyeVec + LightDirection.xyz);
        //float toEyeTheta = pow(acos(dot(N, HalfLV)), SpecularPower);
        //toEyeTheta = dot(PLight.pPower, toEyeTheta);
        //SpecularPower = toEyeTheta;
        
        // 影響度の計算
        float affect = 1.0f - (1.0f / PLight.pRange * Length);
        affect = max(0.0f, affect);
        affect = pow(affect, 3);
        
        float DiffuseFromFresnel = CalcDiffuseFromFresnel(N, L, toEyeVec, Roufness);
    
        // 正規化したLambert拡散反射光を計算(影響度をかける)
        float3 LambertDiffuse = CalcLambertDiffuse(L, PLight.pColor.rgb, N) * affect;
    
        // 最終的な拡散反射光
        float3 diffuse = AlbedoColor.rgb * DiffuseFromFresnel * LambertDiffuse;
    
        // Cook-Trranceモデルを利用した鏡面反射率を計算
        float CTSpecular = CookTrranceSpecular(L, toEyeVec, N, Metallic, MicroFacet, Smooth);
        float3 LightSpecular = PLight.pColor * CTSpecular * affect * SpecularPower;
    
        // 金属度が高ければ、鏡面反射はスペキュラカラー低ければ白に近づく
        LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
        // 滑らかさを使って拡散反射光と鏡面反射光を合成する(滑らかさが高いほど拡散反射は弱くなる)
        lightColor += diffuse * Roufness + LightSpecular;
    }
    // スポットライト
    for (int i = 0; i < SpotLightNum; i++)
    {
        // スポットライト
        SpotLight SLight = sLights[i];
        // ワールド座標からライトの位置に向かうベクトル
        float3 Pixel2Light = SLight.sLightPos - World;
        float3 VPixel2Light = normalize(Pixel2Light);
        float sLen = length(Pixel2Light);
        
        float dotNV = (dot(N, VPixel2Light));
        
        // 影響度計算
        float affect = saturate(1.0f - sLen / SLight.sRange);
        affect = pow(affect, 2.0f);
        // ライトの方向
        float3 SL = normalize(-SLight.sDirection);
        // 間の角度の計算
        float VdotL = dot(VPixel2Light, SL);
        // acos(dot(v1, v2)) = θ
        float angle = acos(VdotL);
        float diffAngle = (SLight.sAngle * 0.5f) * 0.1f;
        float spotAngle = ((SLight.sAngle * 0.5f) + diffAngle);
        float spotRate = (spotAngle - angle) / diffAngle;
        spotRate = pow(saturate(spotRate), 2.0f);
        float FinalRate = spotRate * affect;
        
        lightColor += SLight.sColor * dotNV * affect * spotRate;
        
        // PBR
        // フレネル反射
        //float Fresnel = CalcDiffuseFromFresnel(N, SL, VPixel2Light, Roufness);
        //// ランバート拡散から色を求める
        //float3 diffuse = CalcLambertDiffuse(SL, SLight.sColor.rgb, N) * 5.0f;
        //// 拡散反射光
        //diffuse = diffuse * Fresnel * AlbedoColor.rgb * FinalRate;
        
        //// Cook-Trranceモデルを利用した鏡面反射率を計算
        //float CTSpecular = CookTrranceSpecular(SL, toEyeVec, N, Metallic, MicroFacet, Smooth);
        //float3 LightSpecular = SLight.sColor.rgb * CTSpecular * FinalRate * SpecularPower;
    
        //// 金属度が高ければ、鏡面反射はスペキュラカラー低ければ白に近づく
        //LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
        //// 滑らかさを使って拡散反射光と鏡面反射光を合成する(滑らかさが高いほど拡散反射は弱くなる)
        //lightColor += diffuse * Roufness + LightSpecular;
    }
    
    // 環境光
     lightColor += LightAmbient * AlbedoColor;
    
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    FinalColor.rgb += lightColor;
    //FinalColor.rgb += NlightColor;
    //FinalColor.rgb += ReflectColor.rgb;
    FinalColor.rgb *= BaseColor;
    
	return FinalColor;
}

// ベックマン分布の計算
// ベックマン分布(マイクロファセットの平均的な傾き)
float Beckmann(float _microfacet, float _dotNH)
{
    float t2 = _dotNH * _dotNH;
    float t4 = _dotNH * _dotNH * _dotNH * _dotNH;
    float m2 = _microfacet * _microfacet;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

float GGX(float3 _LDir, float3 _toEye, float3 _Normal, float _Roughness)
{
    // ハーフベクトル
    float3 Half = normalize(_LDir + _toEye);
    // 法線とハーフベクトルの内積計算
    float dotNH = pow(dot(_Normal, Half), 2);
    _Roughness = saturate(_Roughness);
    float alpha = _Roughness * _Roughness;
    float alpha2 = alpha * alpha;
    float t = ((dotNH * dotNH) * (alpha2 - 1.0f) + 1.0f);
    return alpha2 / (PI * t * t);
}

// f 金属度 u ビューベクトルと法線の内積
float SpcFrescel(float f, float u)
{
    return f + (1 - f) * pow(1 - u, 5);
}

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float _Roughness)
{
    // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトル
    float3 H = normalize(L + V);
    
    float energyBias = lerp(0.0f, 0.5f, _Roughness);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, _Roughness);
    
    //光源に向かうベクトルとハーフベクトルの類似度を内積で求める
    float dotLH = saturate(dot(L, H));
    
    // 光源に向かうベクトルとハーフベクトル、光が平行に入射したときの拡散反射量を求める
    float Fd90 = energyBias + 2.0f * dotLH * dotLH * _Roughness;
    
    // 法線と光源に向かうベクトルを利用して拡散反射率を求める
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));
    
    // 法線と視点に向かうベクトルを利用して拡散反射率を求める
    float dotNV = saturate(dot(N, N));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // 法線と光源への方向に依存する拡散反射率と、法線と始点ベクトルに依存する拡散反射率を
    // 乗算して最終的な拡散反射率を求める。
    return (FL * FV * energyFactor);
}

float3 CalcLambertDiffuse(float3 L, float3 Color, float3 N)
{
    // 法線とライトの方向の内積を計算
    float NdotL = saturate(dot(N, L));
    // 内積の値を0以上にする
    float3 lambertDiffuse = Color * NdotL / PI;
    // 拡散反射光の計算
    return lambertDiffuse;
}

float CookTrranceSpecular(float3 L, float3 V, float3 N, float _metallic, float _microfacet, float _smooth)
{
    float metal = _metallic;
    
    // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);
    
    // 各種ベクトルの内積を求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));
    
    //float cell = CalcCellDiffuse(N, L);

    // ベックマン分布を用いてディフューズ項を求める
    float D = Beckmann(_microfacet, NdotH);
    
    // GGX関数からマイクロファセットの値計算
    //float GG = GGX(L, V, N, _Roughness);
    
    //float Facet = lerp(D, GG, _microfacet);
    //float Facet = lerp(GG, D, _blend);
    
    // フレネル項を求める
    float F = SpcFrescel(metal, VdotH);
    
    // 幾何減衰を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
    //float G = min(1.0f, min(2 * NdotH * cell / VdotH, 2 * NdotH * NdotL / VdotH));
    
    // マイクロファセット項を求める
    float m = PI * NdotV * NdotH;
    //float m = PI * cell * NdotH;
    
    // GGXとBeckman分布の線形補間
    //return max(F * Facet * G / m, 0.0f);
    // GGX*Beckman分布
    //return max(F * D *GG * G / m, 0.0f);
    // GGX
    //return max(F * GG * G / m, 0.0f);
    // Beckman分布
    return max(F * D * G / m, 0.0f);
}

float CalcCellDiffuse(float3 _normal, float3 _ldir)
{
    float diffuse = saturate(dot(_normal, _ldir));
    // diffuseの諧調化
    diffuse *= 2.0f;
    diffuse += 0.5f;
    diffuse = floor(diffuse);
    return diffuse /= 2.0f;
}