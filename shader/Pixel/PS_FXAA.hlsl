struct PS_IN    
{
    float4 pos : SV_Position;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

Texture2D tex : register(t1);
SamplerState samp : register(s0);

// RGB�J���[����P�x�v�Z���s��
float Luminance(float3 _color)
{
    return _color.r * 0.299f + _color.g * 0.587f + _color.b * 0.114f;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = tex.Sample(samp, pin.uv);
    
    // �s�N�Z�����ӂŃT���v�����O���邽�߂�UV���W�̍����̌v�Z(��ʃT�C�Y)
    float2 diff = float2(0.5f / 1280.0f, 0.5f / 720.0f);
    // �΂ߕ����̎��ӂ̃s�N�Z���̐F�̋P�x���v�Z
    float lumiLT = Luminance(tex.Sample(samp, pin.uv + diff * float2(-1.0f, 1.0f)).rgb);
    float lumiRT = Luminance(tex.Sample(samp, pin.uv + diff * float2(1.0f, 1.0f)).rgb);
    float lumiLB = Luminance(tex.Sample(samp, pin.uv + diff * float2(-1.0f, -1.0f)).rgb);
    float lumiRB = Luminance(tex.Sample(samp, pin.uv + diff * float2(1.0f, -1.0f)).rgb);
    float lumiC = Luminance(color.rgb);
    
    // ���ӂ̋P�x�̍ő�l�ƍŏ��l�����߂�
    float lumiMax = max(max(lumiLT, lumiRT), max(lumiLB, lumiRB));
    float lumiMin = min(min(lumiLT, lumiRT), min(lumiLB, lumiRB));
    // �ő�P�x�ƍŏ��P�x�̍����v�Z
    float lumiSub = max(lumiMax, lumiC) - min(lumiMin, lumiC);
    
    // �P�x�̍������l�����Ȃ�΁A�G�C���A�X���C������K�v�������Ɣ��f���A���̐F���o��
    //if (lumiSub < 0.125f)
    if (lumiSub < 0.05)
    {
        return color;
    }
    
    // �G�b�W�̕������v�Z
    float dirRT = lumiRT - lumiLB;
    float dirRB = lumiRB - lumiLT;
    float2 dir = float2(dirRT + dirRB, dirRT - dirRB);
    
    // �G�b�W�̕����𐳋K��
    dir = normalize(dir) * diff;
    
    // �G�b�W�̕����ɉ����ĐF���T���v�����O
    float4 col1 = tex.Sample(samp, pin.uv + dir);
    float4 col2 = tex.Sample(samp, pin.uv - dir);
    
    // �T���v�����O�����F���u�����h
    return (col1 + col2) * 0.5f;
}