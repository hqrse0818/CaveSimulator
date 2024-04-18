#pragma once
#include "Com_Camera.h"
#include "../Utility.h"
class Com_FollowCamera :
    public Com_Camera
{
private:
    // ターゲットからどれだけ離れるか
    DirectX::SimpleMath::Vector3 mMultiDistance;
    // 高さの調整
    float fTargetHeight;

    float fMaxHeight = 10.0f;
    float fMinimumHeight = 0.0f;

public:
    Com_FollowCamera()
    {
        mMultiDistance = DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f);
        fTargetHeight = 3.0f;
    }
   
    void SetMultiDistance(DirectX::SimpleMath::Vector3 _val)
    {
        mMultiDistance = _val;
    }
    void SetAddDistance(float _val)
    {
        fTargetHeight = _val;
    }

    void Update()override;
    void PreDraw()override;
};

