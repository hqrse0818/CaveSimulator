#pragma once
#include "Com_RenderCollider.h"
#include "../Format.h"
class Com_RenderCollisionLine_Cylinder :
    public Com_RenderCollider
{
private:
    DirectX::SimpleMath::Vector3 mRenderVertex[48];
    float fRadius;
    float fHeight;
    DirectX::SimpleMath::Vector3 mCenter;
public:
    void Init()override;
    void Update()override;
    void Draw()override;
};

