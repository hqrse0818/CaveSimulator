#include "Com_FollowCamera.h"
#include "../DirectX/renderer.h"
#include "../GameObject/GameObject.h"
#include "../ComponentHeader.h"
#include "../main.h"
#include "../System/Input.h"

using namespace DirectX::SimpleMath;

void Com_FollowCamera::Update()
{
	
}

void Com_FollowCamera::PreDraw()
{
	if (p_mTarget != nullptr)
	{
		// 後ろ向きベクトルを作る
		Vector3 BackVector;
		BackVector.x = p_mTarget->p_mTransform->mForward.x * -1;
		BackVector.y = p_mTarget->p_mTransform->mForward.y;
		BackVector.z = p_mTarget->p_mTransform->mForward.z * -1;

		// ポジションを計算
		Vector3 tmpPos{};
		Vector3 tarpos = p_mTarget->p_mTransform->mPosition;
		tmpPos.x = tarpos.x + BackVector.x * mMultiDistance.x;
		tmpPos.y = tarpos.y + BackVector.y * mMultiDistance.y + fTargetHeight;
		tmpPos.z = tarpos.z + BackVector.z * mMultiDistance.z;

		// ポジションの適応
		p_mObject->p_mTransform->mPosition = tmpPos;

		// 注視点の補正
		mTargetPosition = p_mTarget->p_mTransform->mPosition + p_mTarget->p_mTransform->mForward * mTargetCorrect;

		// 動きが変な奴(保存)
		//TargetPosition = Target->transform->Position * TargetCorrect;

		if (Input::GetKeyState(KEYCODE_UP) == KEYSTATE::KEY_WHILE_DOWN)
		{
			fTargetHeight += 0.1f;
			if (fTargetHeight > fMaxHeight)
			{
				fTargetHeight = fMaxHeight;
			}
		}
		if (Input::GetKeyState(KEYCODE_DOWN) == KEYSTATE::KEY_WHILE_DOWN)
		{
			fTargetHeight -= 0.1f;
			if (fTargetHeight < fMinimumHeight)
			{
				fTargetHeight = fMinimumHeight;
			}
		}
	}
	else
	{
		mTargetPosition = p_mObject->p_mTransform->mPosition + (p_mObject->p_mTransform->mForward * mTargetCorrect);
	}
}