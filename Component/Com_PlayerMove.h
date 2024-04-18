#pragma once
#include "Component.h"

class Com_Camera;

constexpr float BaseMoveSpeed = 30.0f;

class Com_PlayerMove :
    public Component
{
private:
    float fMoveSpeed = BaseMoveSpeed;
    Com_Camera* p_mCameraCom = nullptr;

public:
    void SetMoveSpeed(float _val)
    {
        fMoveSpeed = _val;
    }

    float GetMoveSpeed()
    {
        return fMoveSpeed;
    }

    void SetCameraCom(Com_Camera* _com)
    {
        p_mCameraCom = _com;
    }

    Com_Camera* GetCameraCom()
    {
        return p_mCameraCom;
    }

    void Move(float _x, float _z);
};

