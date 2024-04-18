#include "Com_SphereCollider.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../Debug.h"

using namespace std;

Com_SphereCollider::Com_SphereCollider()
{
	mSphere = new Sphere();
	mType = ColColliderForm::Sphere;
}

void Com_SphereCollider::Init()
{
	mSphere->Center = p_mObject->p_mTransform->mPosition + mCenter;
	mSphere->Radius = fRadius * p_mObject->p_mTransform->mScale.x;
}

void Com_SphereCollider::Update()
{
	mSphere->Center = p_mObject->p_mTransform->mPosition + mCenter;
	mSphere->Radius = fRadius * p_mObject->p_mTransform->mScale.x;
}

void Com_SphereCollider::Uninit()
{
	Com_Collider::Uninit();
	delete mSphere;
}

void Com_SphereCollider::OnCollisionEnter(GameObject* _obj)
{
	NEWDEBUGLOG("Hit" << p_mObject->sObjectName);
}

Com_CircleCollider::Com_CircleCollider()
{
	mCircle = new Circle();
	mType = ColColliderForm::Circle;
}

void Com_CircleCollider::Init()
{
	mCircle->Center.x = p_mObject->p_mTransform->mPosition.x + Center.x;
	mCircle->Center.y = p_mObject->p_mTransform->mPosition.y + Center.y;
	mCircle->Radius = Radius;
}

void Com_CircleCollider::Update()
{
	mCircle->Center.x = p_mObject->p_mTransform->mPosition.x + Center.x;
	mCircle->Center.y = p_mObject->p_mTransform->mPosition.y + Center.y;
	mCircle->Radius = Radius;

	if (p_mObject->mCollisionvector.size() > 0)
	{
		this->bHit = true;
	}
	else
	{
		this->bHit = false;
	}
}

void Com_CircleCollider::Uninit()
{
	Com_Collider::Uninit();
	delete mCircle;
}
