// ����
struct PS_IN
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float4 NormalColor : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
};

// �o��
struct PS_OUT
{
    float4 Normal : SV_Target0; // �@�����J���[�ŏo��
    float4 Color : SV_Target1; // �e�N�X�`���𒣂�t���ďo��
    float4 WorldPos : SV_Target2; // �|�W�V�����̏o��
    float4 ScreenPos : SV_Target3;  //  �X�N���[����|�W�V����
};

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

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float Roughness;
    float Dummy;
};
// �}�e���A���萔�o�b�t�@
cbuffer MaterialBuffer : register(b1)
{
    MATERIAL Material;
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
    float4 EyePos;
    float4 LBaseColor;
    int Num;
    int SpotLightNum;
    float2 LDummy;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    // �@�����J���[�ŏo��
    pout.Normal = (pin.NormalColor / 2.0f) + 0.5f;
    pout.Normal.a = 1.0f;
    // �@����a�̒l�ɃX�y�L�������x��ۑ�
    //pout.Normal.a = Material.Shininess;
    // �|�W�V�������J���[�ŏo��(�h��Ԃ����ꏊ��w��1.0f�ɂ���)
    pout.WorldPos = pin.WorldPos;
    pout.WorldPos.w = 1.0f;
    // �X�y�L�����J���[
    //pout.Specular = Material.Specular;
    // �G�~�b�V����
    //pout.Emissin = Material.Emission;
    //pout.Emissin.a = Material.Roughness;
    
    // �X�N���[����̃|�W�V����
    pout.ScreenPos = pin.Pos;
    
    // �A���x�h
    if (Material.TextureEnable)
    {
        pout.Color = g_Texture.Sample(g_SamplerState, pin.TexCoord);
        pout.Color *= pin.Color;
    }
    else
    {
        pout.Color = pin.Color;
        pout.Color.a = 1.0f;
    }
    
    return pout;
}