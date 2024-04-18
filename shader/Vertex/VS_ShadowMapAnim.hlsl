struct VS_IN
{
    float4 Pos : POSITION0;
    float4 Normal : NORMAL0;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
    int4 BoneIdx : BONEINDEX;
    float4 BoneWeight : BONEWEIGHT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};

struct MATRIXBUFFER
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 Scale;
};

// 行列類の定数バッファ
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
    float4 EyePos;
    float4 Num;
};

// ボーン定数バッファ
cbuffer BoneMatBuf : register(b3)
{
    matrix Bonemtx[400];
}


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    // ワンスキン頂点ブレンドの処理
    float4x4 comb = (float4x4) 0;
    for (int i = 0; i < 4; i++)
    {
		// 重みを計算しながら行列生成
        comb += Bonemtx[vin.BoneIdx[i]] * vin.BoneWeight[i];
    }

    float4 Pos;
    
    Pos = mul(comb, vin.Pos);
    
    vin.Pos = Pos;
    // スケールバッファを計算
    vin.Pos.xyz *= matbuf.Scale.xyz;
    
    vout.pos = mul(vin.Pos, matbuf.World);
    vout.pos = mul(vout.pos, LView);
    vout.pos = mul(vout.pos, LProj);
    
	return vout;
}