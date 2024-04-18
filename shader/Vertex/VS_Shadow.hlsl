// ����
struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

// �o��
struct PS_IN
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
    float4 ShadowPos : TEXCOORD1;
};

struct MATRIXBUFFER
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Scale;
};
// �s��ނ̒萔�o�b�t�@
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

PS_IN main(VS_IN vin)
{
    PS_IN pout;
    // �X�P�[���o�b�t�@�ƌv�Z
    vin.Pos.xyz *= matbuf.Scale.xyz;
    
    // ���C�g�p�@�����v�Z
    float4 worldNormal, normal;
    normal = float4(vin.Normal.xyz, 0.0);
    worldNormal = mul(normal, matbuf.World);
    worldNormal = normalize(worldNormal);
    
   // �@��
    float4 nor = float4(vin.Normal.xyz, 0.0f);
    pout.Normal = normalize(mul(nor, matbuf.World));
    
    // UV���W�̐ݒ�
    pout.TexCoord = vin.TexCoord;
    pout.Color = vin.Diffuse;
    
    // ���W�ϊ�
    matrix wvp;
    wvp = mul(matbuf.World, matbuf.View);
    wvp = mul(wvp, matbuf.Projection);
    pout.Pos = mul(vin.Pos, wvp);
    
    // ���C�g���_�̍��W
    matrix lwvp;
    vin.Pos = mul(vin.Pos, matbuf.World);
    lwvp = mul(matbuf.World, LView);
    lwvp = mul(lwvp, LProj);
    pout.ShadowPos = mul(vin.Pos, lwvp);
    
    return pout;
}