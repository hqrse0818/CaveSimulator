struct VS_IN
{
    float4 pos : POSITION0;
    float4 normal: NORMAL0;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
    uint4 MaterialID : MATERIALID0;
    int4 BoneIdx : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

// �o��
struct VS_OUT
{
    float4 Pos : SV_POSITION0;
    float4 Color : COLOR0;
    float4 NormalColor : COLOR1;
    float2 TexCoord : TEXCOORD0;
    float4 WorldPos : TEXCOORD1;
    uint4 MaterialID : MATERIALID0;
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

// �}�e���A��
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
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
    float4 LBaseColor;
    int Num;
    float3 LDummy;
};

// �{�[���萔�o�b�t�@
cbuffer BoneMatBuf : register(b3)
{
    matrix Bonemtx[400];
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    // �����X�L�����_�u�����h�̏���
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
        // �d�݂��v�Z���Ȃ���s��𐶐�
        comb += Bonemtx[vin.BoneIdx[i]] * vin.BoneWeight[i];
    }
    
    float4 Pos;
    
    Pos = mul(comb, vin.pos);
    
    vin.pos = Pos;
    
    vin.pos.xyz *= matbuf.Scale.xyz;
    
    // �@��
    float4 nor = float4(vin.normal.xyz, 0.0f);
    vout.NormalColor = normalize(mul(-nor, matbuf.World));
    
    // UV���W�̐ݒ�
    vout.TexCoord = vin.uv;
    vout.Color = vin.diffuse;
    vout.MaterialID = vin.MaterialID[0];
    
    // ���[���h���W
    vout.WorldPos = mul(vin.pos, matbuf.World);
    
    // ���W�ϊ�
    matrix wvp;
    wvp = mul(matbuf.World, matbuf.View);
    wvp = mul(wvp, matbuf.Projection);
    vout.Pos = mul(vin.pos, wvp);
    
    return vout;
}