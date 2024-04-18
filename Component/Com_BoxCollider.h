#pragma once
#include "Component.h"
#include "../Utility.h"
#include "../DirectX/renderer.h"
#include "Com_Collider.h"

class Com_BoxCollider :
    public Com_Collider
{
private:
    AABB* mAABB;
public:
    DirectX::SimpleMath::Vector3 mSize = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
    DirectX::SimpleMath::Vector3 mCenter = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    
public:
    Com_BoxCollider();

    void Init();
    void Update();
    void Uninit();

    AABB* GetCollider()
    {
        return mAABB;
    }

    void SetSize(float _x, float _y, float _z)
    {
        mSize.x = _x;
        mSize.y = _y;
        mSize.z = _z;
    }
    void SetCenter(float _x, float _y, float _z)
    {
        mCenter.x = _x;
        mCenter.y = _y;
        mCenter.z = _z;
    }

    DirectX::SimpleMath::Vector3 getmin()
    {
        return mAABB->getmin();
    }

    DirectX::SimpleMath::Vector3 getmax()
    {
        return mAABB->getmax();
    }

    void UpdateAABB();
};

class Com_BoxCollider2D :
    public Com_Collider
{
private:
    Box2D* mBox;

public:
    DirectX::SimpleMath::Vector2 mCenter = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    DirectX::SimpleMath::Vector2 mSize = DirectX::SimpleMath::Vector2(1.0f, 1.0f);

public:
    Com_BoxCollider2D();
    Box2D* GetCollider()
    {
        return mBox;
    }

    void Init()override;
    void Update()override;
    void Uninit();
};