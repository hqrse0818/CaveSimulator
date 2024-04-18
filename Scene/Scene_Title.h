#pragma once
#include "Scene.h"
#include "../ComponentHeader.h"
#include "../GameObject/OBJ_Transition.h"
#include "../DirectX/ShaderResourceView.h"
class Scene_Title :
    public Scene
{
private:
    // 遷移アニメーション用
    OBJ_Transition* p_mTransition{};

    GameObject* Title{};

    bool bLoadScene = false;
    float fFlashCnt = 0.0f;
    Com_Audio* p_mAudio{};

    GameObject* Camera = nullptr;

    GameObject* pPBR;
    Com_Sprite* pPBRSprite;

public:
    //void Init() override;
    //void Start()override;
    //void Update() override;
    //void Draw2D()override;
    //void Uninit()override;

};

