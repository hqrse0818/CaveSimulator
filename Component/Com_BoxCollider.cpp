#include "Com_BoxCollider.h"
#include "../GameObject/GameObject.h"
// コリジョン範囲描画用
#include "Com_RenderCollisionLine_Box.h"
#include "../System/CustomMath.h"

using namespace DirectX::SimpleMath;

Com_BoxCollider::Com_BoxCollider()
{
	mAABB = new AABB();
	mType = ColColliderForm::Box;
}

void Com_BoxCollider::Init()
{
	mAABB->Size = mSize;
#if defined(DEBUG) || defined(_DEBUG)
	// 表示用コンポーネントを追加
	Com_RenderCollisionLine_Box* CollisionLine = new Com_RenderCollisionLine_Box;
	p_mObject->AddComponent(CollisionLine);
#endif // 
}

void Com_BoxCollider::Update()
{
	if (p_mObject->mCollisionvector.size() > 0)
	{
		bHit = true;
	}
	else
	{
		bHit = false;
	}
	// ボックス位置の設定
	mAABB->Center = p_mObject->p_mTransform->mPosition + mCenter;
	mAABB->Size = mSize;
}

void Com_BoxCollider::Uninit()
{
	Com_Collider::Uninit();
	delete mAABB;
}

void Com_BoxCollider::UpdateAABB()
{
	mAABB->Center = p_mObject->p_mTransform->mPosition + mCenter;
	mAABB->Size = mSize;
}

//================//
//					Box2D				   //
//================//
Com_BoxCollider2D::Com_BoxCollider2D()
{
	mBox = new Box2D();
	mType = ColColliderForm::Box2D;
}

void Com_BoxCollider2D::Init()
{
	mBox->Center.x = p_mObject->p_mTransform->mPosition.x + mCenter.x;
	mBox->Center.y = p_mObject->p_mTransform->mPosition.y + mCenter.y;
	mBox->Size.x = mSize.x;
	mBox->Size.y = mSize.y;
#if defined(DEBUG) || defined(_DEBUG)
	// 表示用コンポーネントを追加
	Com_RenderCollisionLine_Box2D* CollisionLine = new Com_RenderCollisionLine_Box2D();
	p_mObject->AddComponent(CollisionLine);
#endif // 
}

void Com_BoxCollider2D::Update()
{
	mBox->Center.x = p_mObject->p_mTransform->mPosition.x + mCenter.x;
	mBox->Center.y = p_mObject->p_mTransform->mPosition.y + mCenter.y;
	mBox->Size.x = mSize.x;
	mBox->Size.y = mSize.y;
}

void Com_BoxCollider2D::Uninit()
{
	Com_Collider::Uninit();
	delete mBox;
}
