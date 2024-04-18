#pragma once
#include "Component.h"
#include "../Format/Light.h"

// バネの動きをするライト

constexpr float AccelaratePermissionVal = 0.1f;

class Com_SpringLight :
    public Component
{
private:    // 変数
    enum class SpringState
    {
        Start, Do, End
    };
    PointLight* p_mLight;
    // 初期位置(自然長が加算される)
    DirectX::SimpleMath::Vector3 mInitPos = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    // ベクトル
    DirectX::SimpleMath::Vector3 mVelocity{};
    // 重力
    DirectX::SimpleMath::Vector3 mGravity{};
    // 基底表示位置
    DirectX::SimpleMath::Vector3 mAxis{};
    // バネのステート
    SpringState mState = SpringState::Start;

    // バネの自然長
    float mSpringLength = 30.0f;
    // 動かす物体の質量
    float mMass = 10.0f;
    // バネ定数
    float mSpringCoefficient = 5.0f;
    // 抵抗係数
    float mAtt = 3.0f;

public: // 固有関数
    Com_SpringLight();

    // 初期位置設定
    void SetInitPos(float _x, float _y, float _z)
    {
        mInitPos.x = _x;
        mInitPos.y = _y;
        mInitPos.z = _z;
    }
    // 重力のセット
    void SetGravity(float _x, float _y, float _z)
    {
        mGravity.x = _x;
        mGravity.y = _y;
        mGravity.z = _z;
    }
    // ポジション基底位置のセット
    void SetAxis(float _x, float _y, float _z)
    {
        mAxis.x = _x;
        mAxis.y = _y;
        mAxis.z = _z;
    }
    // ライトのカラーのセット
    void SetColor(float _r, float _g, float _b)
    {
        p_mLight->Diffuse.x = _r;
        p_mLight->Diffuse.y = _g;
        p_mLight->Diffuse.z = _b;
    }
    // ライトの範囲のセット
    void SetRange(float _val)
    {
        p_mLight->Position.w = _val;
    }
    // ライトの強さのセット
    void SetLightPower(float _val)
    {
        p_mLight->Diffuse.w = _val;
    }
    // バネの自然長のセット
    void SetSpringLength(float _val)
    {
        mSpringLength = _val;
    }
    // 物体の重さのセット
    void SetMass(float _val)
    {
        mMass = _val;
    }
    // バネ定数のセット
    void SetSpringCoefficient(float _val)
    {
        mSpringCoefficient = _val;
    }
    //　抵抗係数のセット
    void SetAtt(float _val)
    {
        mAtt = _val;
    }

protected:
    void SpringStart();
    void SpringUpdate();

public: // 継承関数
    void Init()override;
    void Update()override;
};

