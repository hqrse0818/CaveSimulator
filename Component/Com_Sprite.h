#pragma once
#include "Com_Visual.h"
#include "../Format.h"
#include "../DirectX/ShaderResourceView.h"
#include <d3d11.h>

constexpr int SpriteVertexNum = 4;

class Com_Sprite :
    public Com_Visual
{
protected:
    ID3D11Buffer* p_mVertexBuffer{};
    ShaderResourceView* p_mSRV{};

    bool bUpdate = false;
    bool bRelease = true;

    MATERIAL mMaterial{};
    DirectX::SimpleMath::Color mColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
    DirectX::SimpleMath::Vector2 mSize = DirectX::SimpleMath::Vector2(10.0f, 10.0f);

    int iSeparateX = 1;
    int iSeparateY = 1;

    float fCurrentx = 0;
    float fCurrenty = 0;
    float fCurrentz = 1;
    float fCurrentw = 1;

    int iCurrent = 1;

public:

    Com_Sprite();

    virtual void Init();
    virtual void Uninit();
    virtual void Update();
    virtual void Draw();

    // 頂点位置変更やアニメーションの有無
    void SetUpdate(bool _val)
    {
        bUpdate = _val;
    };
    // 削除時SRVも一緒にリリースするか
    void SetRelease(bool _val = false)
    {
        bRelease = _val;
    }

    void SetSeparateNum(int _u, int _v);
    void SetCurrent(int _val);

    void SetTexture(const char* _name);
    void SetTexture(ShaderResourceView* _psrv);
    void SetSize(float _x, float _y);
    void SetVertexColor(float _r, float _g, float _b, float _a);
    void SetMaterial(MATERIAL mat);
    void SetAllbedoColor(DirectX::SimpleMath::Color _color);
    void SetEmission(DirectX::SimpleMath::Color _color);
    virtual void Create();

    ID3D11ShaderResourceView* GetSRV();


    MATERIAL GetMaterial()
    {
        return mMaterial;
    }
};

