#pragma once
#include "Component.h"
#include "../DirectX/Shader.h"

#define VS_DEFAULTGBUFFER ("VS_GBufferPBR")
#define PS_DEFAULTGBUFFER ("VS_GBufferPBR")

class Com_Visual :
    public Component
{
protected:
    VertexShader* p_VS = nullptr;
    PixelShader* p_PS = nullptr;
    const char* VSName = "";
    const char* PSName = "";
    DirectX::SimpleMath::Vector4 mScale = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

public:
    Com_Visual() {};

    virtual void ShadowDraw()override;
    void UpdateScale(float _x, float _y, float _z);

    void SetVertex(const char* _name);
    void SetPixel(const char* _name);

    void SetVertexGBuffer(const char* _name =VS_DEFAULTGBUFFER);
    void SetPixelGbuffer(const char* _name = PS_DEFAULTGBUFFER);

    VertexShader* GetVS()
    {
        return p_VS;
    }
    PixelShader* GetPS()
    {
        return p_PS;
    }
    virtual void Bind();
    bool bShadowEnable = true;
};

