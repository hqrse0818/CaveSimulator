#pragma once
#include "Component.h"
#include "../Utility.h"


class Com_Camera :
    public Component
{
protected:
    // ターゲット
    GameObject* p_mTarget{};
    // 
    DirectX::SimpleMath::Vector3 mUp = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
    // ビュー変換行列
    DirectX::SimpleMath::Matrix mViewMatrix{};

    // プロジェクション変換行列
    DirectX::SimpleMath::Matrix mProjectionMatrix{};

    // 最終的な注視点
    DirectX::SimpleMath::Vector3 mTargetPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    // 注視点の調整
    // 値が大きいほど元の注視点の先を見る
    DirectX::SimpleMath::Vector3 mTargetCorrect = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

    // オブジェクトをターゲットにする
    bool bUseTargetObject = true;
    float fFov = 45.0f;
    float fNear = 1.0f;
    float fFar = 1000.0f;



public:
    // オブジェクトの位置をターゲットにする場合
    void SetTarget(GameObject* _targetpos);
    // ポジション自体をターゲットにする
    void SetTargetPosition(DirectX::SimpleMath::Vector3 _pos)
    {
        mTargetPosition = _pos;
    }
    // オブジェクトターゲットを有効にする
    void SetUseTarget(bool _val)
    {
        bUseTargetObject = _val;
    }
    // 注視点の調整
    void SetCorrect(DirectX::SimpleMath::Vector3 _val)
    {
        mTargetCorrect = _val;
    }
    // 
    void SetUp(DirectX::SimpleMath::Vector3 _Up)
    {
        mUp = _Up;
    }
    // 視野角の調整
    void SetFov(float _fov)
    {
        fFov = _fov;
    }
    // 見える距離の調整
    void SetNear(float _near)
    {
        fNear = _near;
    }
    // 見える距離の調整
    void SetFar(float _far)
    {
        fFar = _far;
    }

    const DirectX::SimpleMath::Matrix GetViewMatrix()
    {
        return mViewMatrix;
    }
    const DirectX::SimpleMath::Matrix GetProjection()
    {
        return mProjectionMatrix;
    }

    const DirectX::SimpleMath::Vector3 GetTargetPosition()
    {
        return mTargetPosition;
    }
    const DirectX::SimpleMath::Vector3 GetUp()
    {
        return mUp;
    }

    void Init();
    virtual void Update();
    virtual void Draw();
};

