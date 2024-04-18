#pragma once
#include "Component.h"
#include "../DirectX/Shader.h"

// 旧式バージョン(Com_Visualに組み込み済み)

class Com_Shader :
    public Component
{
public:
    VertexShader* p_mVS{};
    PixelShader* p_mPS{};

public:
    Com_Shader();
    void Uninit()override;
    void Draw()override;
};

