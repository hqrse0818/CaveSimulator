#include "Com_SimpleTranslate.h"
#include "../GameObject/GameObject.h"
#include "../System/CustomMath.h"
#include "../System/Time.h"

using namespace DirectX::SimpleMath;

void Com_SimpleTranslate::SetMoveSpeed(float _val)
{
	fMoveSpeed = _val;
}

void Com_SimpleTranslate::SetCamera(GameObject* _obj)
{
	p_mCamera = _obj;
}

void Com_SimpleTranslate::Update()
{
	mMoveDirection = p_mObject->GetPosition() - p_mCamera->GetPosition();
	mMoveDirection.Normalize();
}

void Com_SimpleTranslate::Move(float _x, float _z)
{
	// 正面ベクトル
	Vector3 forward = mMoveDirection;
	// 右向きベクトル
	Vector3 right = Math::GetCross(Vector3::Up, forward);
	forward *= _x * fMoveSpeed * Time->GetDeltaTime();
	forward.y = 0.0f;
	right *= _z * fMoveSpeed * Time->GetDeltaTime();
	right.y = 0.0f;

	Vector3 Velocity = forward + right;
	p_mObject->p_mTransform->Translate(Velocity);
}
