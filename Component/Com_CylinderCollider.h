#pragma once
#include "Component.h"
#include "Com_Collider.h"
#include "../Utility.h"

class Com_CylinderCollider :
    public Com_Collider
{
    Cylinder* mCylinder;

    float fBottomLength = 1.0f;
    float fTopLength = 1.0f;
public:
    Com_CylinderCollider();

    
    Cylinder* GetCollider()
    {
        return mCylinder;
    }

    void Init()override;
    void Update()override;
    void Uninit();
};

