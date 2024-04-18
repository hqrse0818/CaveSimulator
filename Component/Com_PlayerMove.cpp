#include "Com_PlayerMove.h"
#include "../System/Time.h"
#include "../GameObject/GameObject.h"
#include "../Component/Com_Camera.h"
#include "../System/CustomMath.h"

using namespace DirectX::SimpleMath;

void Com_PlayerMove::Move(float _x, float _z)
{
	// カメラの正面ベクトルを計算
	Vector3 Target = p_mCameraCom->GetTargetPosition();
	Vector3 CamPos = p_mCameraCom->p_mObject->p_mTransform->mPosition;
	Vector3 Forward = Math::GetVector(CamPos, Target);
	// 正規化する
	Forward = Math::Normalize(Forward);
	// 右向きベクトルを抜き出す
	Vector3 Right = Math::GetCross(Vector3::Up, Forward);

	Forward *= _z * fMoveSpeed * Time->GetDeltaTime();
	Forward.y = 0.0f;
	// 入力から移動量を生成
	Right *= _x * fMoveSpeed * Time->GetDeltaTime();
	Right.y = 0.0f;

	// 動く方向&向きたい方向
	Vector3 Velocity = Forward + Right;
	//Velocity = Math::Normalize(Velocity);
	p_mObject->p_mTransform->Translate(Velocity);

	if (Velocity.x == 0 && Velocity.z == 0)
	{
		return;
	}

	Vector3 v = Math::Normalize(Velocity);
	float angle = atan2f(v.x, v.z);
	p_mObject->p_mTransform->mRotation.y = angle;
}
