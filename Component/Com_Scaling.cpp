#include "Com_Scaling.h"
#include "../System/Time.h"
#include "../GameObject/GameObject.h"

using namespace DirectX::SimpleMath;

void Com_Scaling::Update()
{
	if (bReturn)
	{
		CurrentScale += Time->GetDeltaTime();
		if (CurrentScale >= MaxScale)
		{
			CurrentScale = MaxScale;
			bReturn = false;
		}
	}
	else
	{
		CurrentScale -= Time->GetDeltaTime();
		if (CurrentScale <= MinScale)
		{
			CurrentScale = MinScale;
			bReturn = true;
		}
	}

	p_mObject->SetScale(InitScale * CurrentScale);
}

void Com_Scaling::Init()
{
	InitScale = p_mObject->p_mTransform->mScale;
}
