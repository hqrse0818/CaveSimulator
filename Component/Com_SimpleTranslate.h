#pragma once
#include "Component.h"
#include "../Utility.h"
// カメラの向きを元に移動するクラス

constexpr float SimpleTransComBaseSpeed = 5.0f;

class Com_SimpleTranslate :
    public Component
{
private:
    float fMoveSpeed = SimpleTransComBaseSpeed;
    GameObject* p_mCamera = nullptr;
    DirectX::SimpleMath::Vector3 mMoveDirection;

public:
    void SetMoveSpeed(float _val);
    void SetCamera(GameObject* _obj);
    void Move(float _x, float _z);
    void Update();
};