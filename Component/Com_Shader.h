#pragma once
#include "Component.h"
#include "../DirectX/Shader.h"

// �����o�[�W����(Com_Visual�ɑg�ݍ��ݍς�)

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

