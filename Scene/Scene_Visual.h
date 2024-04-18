#pragma once
#include "Scene.h"
#include "../GameObject/OBJ_Player.h"
class Scene_Visual :
    public Scene
{
private:

public:
    void Init();
    void Update()override;
    void Uninit();
};

