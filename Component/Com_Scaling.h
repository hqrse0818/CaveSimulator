#pragma once
#include "Component.h"
#include "../Utility.h"

constexpr float ScaleComBaseMaxScale = 1.2f;
constexpr float ScaleComBaseMinScale = 0.8f;

class Com_Scaling :
    public Component
{
public:
    float MaxScale = ScaleComBaseMaxScale;
    float MinScale = ScaleComBaseMinScale;
    float CurrentScale = 1.0f;

    bool bReturn = false;

    DirectX::SimpleMath::Vector3 InitScale;

public:
    void Init();
    void Update()override;
};

