#pragma once
#include "Com_Visual.h"
#include <d3d11.h>
#include <SimpleMath.h>
#include "../Format.h"
#include "../DirectX/ShaderResourceView.h"

// 3Dのスプライト

class Com_3DSprite :
    public Com_Visual
{
protected:
    ID3D11Buffer* p_mVertexBuffer{};
    ShaderResourceView* p_mSRV{};
    MATERIAL                    p_mMaterial{};

    // テクスチャのリピート回数
    float fMaxU = 10.0f;
    float fMaxV = 10.0f;

    // テクスチャサイズ
    DirectX::SimpleMath::Vector2 mSize = DirectX::SimpleMath::Vector2(50.0f, 50.0f);

public:

    Com_3DSprite();
    virtual void Init() override;
    virtual void Uninit() override;
    virtual void Update() override;
    virtual void Draw() override;

    virtual void SetTexture(const char* _name);
    virtual void SetMaterial(MATERIAL mat);

    void SetUV(float _u, float _v);

    void SetSize(float _x, float _y)
    {
        mSize.x = _x;
        mSize.y = _y;
    }
};