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
// �s��ނ̒萔�o�b�t�@
cbuffer Matrix0 : register(b0)
{
    MATRIXBUFFER matbuf;
}

// �f�B���N�V���i�����C�g�萔�o�b�t�@
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



// �|�C���g���C�g��`
struct PointLight
{
    float3 pLightPos;
    float pRange;
    float3 pColor;
    float pPower;
};

// �|�C���g���C�g�萔�o�b�t�@
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

// �x�b�N�}�����z�̌v�Z
float Beckmann(float m, float t);
// GGX�֐�
float GGX(float3 _LDir, float3 _toEye, float3 _Normal, float _Roughness);
// �t���l���̌v�Z
float SpcFrescel(float f, float u);
// �t���l�����˂��l�������g�U���ˌ������߂�
float CalcDiffuseFromFresnel(float3 _normal, float3 _ldir, float3 _toEye, float _Roughness);
// �����o�[�g�g�U���ˌ������߂�
float3 CalcLambertDiffuse(float3 _lightDirection, float3 _lightColor, float3 _normal);
// Cook-Trrance���f���̋��ʔ��ˌv�Z
float CookTrranceSpecular(float3 _LDir, float3 _toEye, float3 _normal, float _metallic, float _microfacet, float _smooth);
// �Z�����v�Z
float CalcCellDiffuse(float3 _normal, float3 _ldir);

float4 main(PS_IN pin) : SV_TARGET
{
    // �@��
    float3 Normal = NormalTex.Sample(samp, pin.uv).xyz;
    float3 N = normalize(Normal);
    // ���[���h���W
    float3 World = WorldPosTex.Sample(samp, pin.uv).xyz;
    // �A���x�h�J���[
    float4 AlbedoColor = AlbedoTex.Sample(samp, pin.uv);
    // �X�y�L�����J���[
    float3 SpecularColor = AlbedoColor.rgb;
    
    // �}�e���A��ID
    uint MatID = uint(MaterialTex.Sample(samp, pin.uv).r * 65535.0f);
    
    // �x�[�X�F
    float3 BaseColor = Materials[MatID].Aldbedo.rgb;
    // �����x
    float Metallic = Materials[MatID].Metalic;
    // ���炩��
    float Roufness = Materials[MatID].Roufhness;
    float Smooth = 1.0f - Roufness;
    
    // �����\��
    float MicroFacet = Materials[MatID].MicroFacet;
    // �X�y�L�������x
    float SpecularPower = Materials[MatID].Shiness;
    
    // �����Ɍ������ĐL�т�x�N�g��
    float3 toEyeVec = normalize(matbuf.CameraPos.xyz - World);
    
    // ���˃x�N�g��
    //float3 ReflectDir = reflect(-toEyeVec, Normal);
    //float4 ReflectColor = CubeTex.SampleLevel(samp, ReflectDir, Roufness * 5) * Metallic;
    //float4 ReflectColor = CubeTex.Sample(samp1, ReflectDir) * Metallic;
    
    // �X�y�L�������x�^�ʖڌv�Z?
    //float3 HalfLV = normalize(toEyeVec + LightDirection.xyz);
    //float toEyeTheta = pow(acos(dot(N, HalfLV)), SpecularPower);
    //toEyeTheta = dot(LightPower, toEyeTheta);
    //SpecularPower = toEyeTheta;
    // �����܂�
    
    float3 lightColor = 0;
    float3 NlightColor = 0;
    
    // ----------------------- //
    // ���s�����̃��C�e�B���O //
    // ----------------------- //
    float DiffuseFromFresnel = CalcDiffuseFromFresnel(N, -LightDirection.xyz, toEyeVec, Roufness);
    
    // ���K������Lambert�g�U���ˌ����v�Z
    float3 LambertDiffuse = CalcLambertDiffuse(-LightDirection.xyz, LightDiffuse, N);
    
    // �ŏI�I�Ȋg�U���ˌ�
    float3 diffuse = AlbedoColor.rgb * DiffuseFromFresnel * LambertDiffuse;
    
    // Cook-Trrance���f���𗘗p�������ʔ��˗����v�Z
    float CTSpecular = CookTrranceSpecular(-LightDirection.xyz, toEyeVec, N, Metallic, MicroFacet, Smooth);
    float3 LightSpecular = LightDiffuse * CTSpecular * SpecularPower;
    
    // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�Ⴏ��Δ��ɋ߂Â�
    LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
    // ���炩�����g���Ċg�U���ˌ��Ƌ��ʔ��ˌ�����������(���炩���������قǊg�U���˂͎キ�Ȃ�)
    lightColor += diffuse * Roufness + LightSpecular;
    //NlightColor += AlbedoColor.rgb * (CalcCellDiffuse(N, -LightDirection.xyz) + LightAmbient.rgb);
    
    // ---------------------------- //
    // �|�C���g���C�g�̃��C�e�B���O //
    // ---------------------------- //
    for (int i = 0; i < PointLightNum; i++)
    {
        // �|�C���g���C�g
        PointLight PLight = pLights[i];
        // ���[���h���W����|�C���g���C�g�Ɍ������x�N�g��
        float3 toPointLightDirection = PLight.pLightPos.xyz - World;
        float3 L = normalize(toPointLightDirection);
        // ����
        float Length = length(toPointLightDirection);
        
        // �X�y�L�������x�^�ʖڌv�Z?
        //float3 HalfLV = normalize(toEyeVec + LightDirection.xyz);
        //float toEyeTheta = pow(acos(dot(N, HalfLV)), SpecularPower);
        //toEyeTheta = dot(PLight.pPower, toEyeTheta);
        //SpecularPower = toEyeTheta;
        
        // �e���x�̌v�Z
        float affect = 1.0f - (1.0f / PLight.pRange * Length);
        affect = max(0.0f, affect);
        affect = pow(affect, 3);
        
        float DiffuseFromFresnel = CalcDiffuseFromFresnel(N, L, toEyeVec, Roufness);
    
        // ���K������Lambert�g�U���ˌ����v�Z(�e���x��������)
        float3 LambertDiffuse = CalcLambertDiffuse(L, PLight.pColor.rgb, N) * affect;
    
        // �ŏI�I�Ȋg�U���ˌ�
        float3 diffuse = AlbedoColor.rgb * DiffuseFromFresnel * LambertDiffuse;
    
        // Cook-Trrance���f���𗘗p�������ʔ��˗����v�Z
        float CTSpecular = CookTrranceSpecular(L, toEyeVec, N, Metallic, MicroFacet, Smooth);
        float3 LightSpecular = PLight.pColor * CTSpecular * affect * SpecularPower;
    
        // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�Ⴏ��Δ��ɋ߂Â�
        LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
        // ���炩�����g���Ċg�U���ˌ��Ƌ��ʔ��ˌ�����������(���炩���������قǊg�U���˂͎キ�Ȃ�)
        lightColor += diffuse * Roufness + LightSpecular;
    }
    // �X�|�b�g���C�g
    for (int i = 0; i < SpotLightNum; i++)
    {
        // �X�|�b�g���C�g
        SpotLight SLight = sLights[i];
        // ���[���h���W���烉�C�g�̈ʒu�Ɍ������x�N�g��
        float3 Pixel2Light = SLight.sLightPos - World;
        float3 VPixel2Light = normalize(Pixel2Light);
        float sLen = length(Pixel2Light);
        
        float dotNV = (dot(N, VPixel2Light));
        
        // �e���x�v�Z
        float affect = saturate(1.0f - sLen / SLight.sRange);
        affect = pow(affect, 2.0f);
        // ���C�g�̕���
        float3 SL = normalize(-SLight.sDirection);
        // �Ԃ̊p�x�̌v�Z
        float VdotL = dot(VPixel2Light, SL);
        // acos(dot(v1, v2)) = ��
        float angle = acos(VdotL);
        float diffAngle = (SLight.sAngle * 0.5f) * 0.1f;
        float spotAngle = ((SLight.sAngle * 0.5f) + diffAngle);
        float spotRate = (spotAngle - angle) / diffAngle;
        spotRate = pow(saturate(spotRate), 2.0f);
        float FinalRate = spotRate * affect;
        
        lightColor += SLight.sColor * dotNV * affect * spotRate;
        
        // PBR
        // �t���l������
        //float Fresnel = CalcDiffuseFromFresnel(N, SL, VPixel2Light, Roufness);
        //// �����o�[�g�g�U����F�����߂�
        //float3 diffuse = CalcLambertDiffuse(SL, SLight.sColor.rgb, N) * 5.0f;
        //// �g�U���ˌ�
        //diffuse = diffuse * Fresnel * AlbedoColor.rgb * FinalRate;
        
        //// Cook-Trrance���f���𗘗p�������ʔ��˗����v�Z
        //float CTSpecular = CookTrranceSpecular(SL, toEyeVec, N, Metallic, MicroFacet, Smooth);
        //float3 LightSpecular = SLight.sColor.rgb * CTSpecular * FinalRate * SpecularPower;
    
        //// �����x��������΁A���ʔ��˂̓X�y�L�����J���[�Ⴏ��Δ��ɋ߂Â�
        //LightSpecular *= lerp(float3(1.0f, 1.0f, 1.0f), SpecularColor, Metallic);
    
        //// ���炩�����g���Ċg�U���ˌ��Ƌ��ʔ��ˌ�����������(���炩���������قǊg�U���˂͎キ�Ȃ�)
        //lightColor += diffuse * Roufness + LightSpecular;
    }
    
    // ����
     lightColor += LightAmbient * AlbedoColor;
    
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    FinalColor.rgb += lightColor;
    //FinalColor.rgb += NlightColor;
    //FinalColor.rgb += ReflectColor.rgb;
    FinalColor.rgb *= BaseColor;
    
	return FinalColor;
}

// �x�b�N�}�����z�̌v�Z
// �x�b�N�}�����z(�}�C�N���t�@�Z�b�g�̕��ϓI�ȌX��)
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
    // �n�[�t�x�N�g��
    float3 Half = normalize(_LDir + _toEye);
    // �@���ƃn�[�t�x�N�g���̓��όv�Z
    float dotNH = pow(dot(_Normal, Half), 2);
    _Roughness = saturate(_Roughness);
    float alpha = _Roughness * _Roughness;
    float alpha2 = alpha * alpha;
    float t = ((dotNH * dotNH) * (alpha2 - 1.0f) + 1.0f);
    return alpha2 / (PI * t * t);
}

// f �����x u �r���[�x�N�g���Ɩ@���̓���
float SpcFrescel(float f, float u)
{
    return f + (1 - f) * pow(1 - u, 5);
}

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V, float _Roughness)
{
    // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g��
    float3 H = normalize(L + V);
    
    float energyBias = lerp(0.0f, 0.5f, _Roughness);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, _Roughness);
    
    //�����Ɍ������x�N�g���ƃn�[�t�x�N�g���̗ގ��x����ςŋ��߂�
    float dotLH = saturate(dot(L, H));
    
    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A�������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂�
    float Fd90 = energyBias + 2.0f * dotLH * dotLH * _Roughness;
    
    // �@���ƌ����Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(N, L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));
    
    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(N, N));
    float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝn�_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂�B
    return (FL * FV * energyFactor);
}

float3 CalcLambertDiffuse(float3 L, float3 Color, float3 N)
{
    // �@���ƃ��C�g�̕����̓��ς��v�Z
    float NdotL = saturate(dot(N, L));
    // ���ς̒l��0�ȏ�ɂ���
    float3 lambertDiffuse = Color * NdotL / PI;
    // �g�U���ˌ��̌v�Z
    return lambertDiffuse;
}

float CookTrranceSpecular(float3 L, float3 V, float3 N, float _metallic, float _microfacet, float _smooth)
{
    float metal = _metallic;
    
    // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
    float3 H = normalize(L + V);
    
    // �e��x�N�g���̓��ς����߂�
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));
    
    //float cell = CalcCellDiffuse(N, L);

    // �x�b�N�}�����z��p���ăf�B�t���[�Y�������߂�
    float D = Beckmann(_microfacet, NdotH);
    
    // GGX�֐�����}�C�N���t�@�Z�b�g�̒l�v�Z
    //float GG = GGX(L, V, N, _Roughness);
    
    //float Facet = lerp(D, GG, _microfacet);
    //float Facet = lerp(GG, D, _blend);
    
    // �t���l���������߂�
    float F = SpcFrescel(metal, VdotH);
    
    // �􉽌��������߂�
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
    //float G = min(1.0f, min(2 * NdotH * cell / VdotH, 2 * NdotH * NdotL / VdotH));
    
    // �}�C�N���t�@�Z�b�g�������߂�
    float m = PI * NdotV * NdotH;
    //float m = PI * cell * NdotH;
    
    // GGX��Beckman���z�̐��`���
    //return max(F * Facet * G / m, 0.0f);
    // GGX*Beckman���z
    //return max(F * D *GG * G / m, 0.0f);
    // GGX
    //return max(F * GG * G / m, 0.0f);
    // Beckman���z
    return max(F * D * G / m, 0.0f);
}

float CalcCellDiffuse(float3 _normal, float3 _ldir)
{
    float diffuse = saturate(dot(_normal, _ldir));
    // diffuse���~����
    diffuse *= 2.0f;
    diffuse += 0.5f;
    diffuse = floor(diffuse);
    return diffuse /= 2.0f;
}