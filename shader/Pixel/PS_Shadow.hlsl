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
    
    // �e�Ŏg��UV���v�Z
    float2 depthUV = pin.shadowPos.xy / pin.shadowPos.w;
    // �[�x�l���擾
    float pixelDepth = pin.shadowPos.z / pin.shadowPos.w;
    
    // UV���W��-1����1�͈̔͂���0����1�͈̔͂ɕϊ���Y���W�̌����𔽓]
    depthUV = max(-1.0f, min(1.0f, depthUV)) * 0.5f + 0.5f;
    depthUV.y = -depthUV.y;
    
    // �[�x�e�N�X�`������[�x�̒l���擾
    float depth = depthTex.Sample(samp, depthUV);
    
    // �擾�����[�x�l����e�̕␳���s��
    color.rgb *= saturate(step(pixelDepth, depth) + 0.3f);
    
    return color;
}