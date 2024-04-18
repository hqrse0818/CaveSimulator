#include "Com_CylinderCollider.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

Com_CylinderCollider::Com_CylinderCollider()
{
	mCylinder = new Cylinder();
	mType = ColColliderForm::Cylinder;
}

void Com_CylinderCollider::Init()
{
	
}

void Com_CylinderCollider::Update()
{
	mCylinder->Center = p_mObject->p_mTransform->mPosition;
}

void Com_CylinderCollider::Uninit()
{
	Com_Collider::Uninit();
	delete mCylinder;
}
