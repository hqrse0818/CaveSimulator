#pragma once
#include "Scene.h"
class Scene_ShaderLoad :
    public Scene
{
public:
    void PreInit()override;
    void Update();
};

