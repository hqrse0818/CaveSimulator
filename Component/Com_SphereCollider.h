#pragma once
#include "Component.h"
#include "Com_Collider.h"
#include "../Utility.h"

class Com_SphereCollider :
    public Com_Collider
{
private:
    Sphere* mSphere;
public:
    float fRadius = 1.0f;
    DirectX::SimpleMath::Vector3 mCenter = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
public:
    Com_SphereCollider();
    
    Sphere* GetCollider()
    {
        return mSphere;
    }

    void Init()override;
    void Update()override;
    void Uninit();

    void OnCollisionEnter(GameObject* _obj)override;
};

class Com_CircleCollider :
    public Com_Collider
{
private:
    Circle* mCircle;

public:

    Com_CircleCollider();

    DirectX::SimpleMath::Vector2 Center = DirectX::SimpleMath::Vector2(0.0f, 0.0f);
    float Radius = 1.0f;

    void Init();
    void Update();
    void Uninit();

    Circle* GetCollider()
    {
        return mCircle;
    }
};

