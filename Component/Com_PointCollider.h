#pragma once
#include "Com_Collider.h"
#include "../Format.h"
// ポイントコライダー
class Com_PointCollider :
    public Com_Collider
{
protected:
    Point3D* mPoint;

public:
    DirectX::SimpleMath::Vector3 mCenter = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);

public:
    Com_PointCollider();
    void Init();
    void Update();
    void Uninit();

    void SetCenter(float _x, float _y, float _z)
    {
        mCenter.x = _x;
        mCenter.y = _y;
        mCenter.z = _z;
    }

    Point3D* GetCollider()
    {
        return mPoint;
    }
};

