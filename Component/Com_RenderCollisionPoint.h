#pragma once
#include "Com_RenderCollider.h"
#include "../Utility.h"
#include "../DirectX/renderer.h"
class Com_RenderCollisionPoint :
    public Com_RenderCollider
{
private:
    DirectX::SimpleMath::Vector3 mRenderVertex;

public:
    void Init();
    void Update();
    void Draw();
};

