#pragma once
#include "GameObject.h"
#include "../Component/Com_Sprite.h"

// フェードシーン遷移用オブジェクト
class OBJ_Transition :
    public GameObject
{
public:
    enum class FadeState
    {
        Stop,
        In,
        Out,
        Finish
    };

private:
    DirectX::SimpleMath::Color mColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
    FadeState mState = FadeState::Stop;
    float FadeTime = 0.0f;
    Com_Sprite* Sprite_buf{};

public:

    OBJ_Transition();
    OBJ_Transition(const char* _name);
    void Update()override;

    FadeState GetState()
    {
        return mState;
    }

    void FadeIn(float _time)
    {
        mColor.w = 1.0f;
        FadeTime = _time;
        mState = FadeState::In;
    }

    void FadeOut(float _time)
    {
        mColor.w = 0.0f;
        FadeTime = _time;
        mState = FadeState::Out;
    }

    void SetColor(DirectX::SimpleMath::Vector3 _Col)
    {
        mColor.x = _Col.x;
        mColor.y = _Col.y;
        mColor.z = _Col.z;
    }
    void SetColor(DirectX::SimpleMath::Color _Col)
    {
        mColor = _Col;
    }
};

