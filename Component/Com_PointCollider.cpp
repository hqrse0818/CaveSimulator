#include "Com_PointCollider.h"
#include "../GameObject/GameObject.h"

using namespace DirectX::SimpleMath;

Com_PointCollider::Com_PointCollider()
{
	mPoint = new Point3D();
}

void Com_PointCollider::Init()
{
	mPoint->Center = mCenter + p_mObject->p_mTransform->mPosition;
}

void Com_PointCollider::Update()
{
	mPoint->Center = mCenter + p_mObject->p_mTransform->mPosition;
}

void Com_PointCollider::Uninit()
{
	delete mPoint;
}
