#include "OBJ_Player.h"
#include "../Scene/Scene.h"
#include "../System/Input.h"
#include "../System/Time.h"
#include "../System/SkeletalModelPool.h"

using namespace DirectX::SimpleMath;

OBJ_Player::OBJ_Player()
{
	p_mModelCom = new Com_SkeletalMesh();
	p_mModelCom->SetModelData("Player");
	p_mModelCom->SetBlendRate(0.25f);
	//p_mModelCom->SetVertex("VS_AnimModel");
	p_mModelCom->SetVertex("VS_GBufferAnim");
	//p_mModelCom->SetPixel("PS_MODEL");
	p_mModelCom->SetPixel("PS_GBuffer");
	p_mModelCom->UpdateScale(0.01f, 0.01f, 0.01f);
	p_mModelCom->SetUseMaterial(true);
	SkeletalMeshLoader* data = SkeletalModelPool::GetLoaderData("Player");
	data->SetMetallic(1.0f, 0);
	AddComponent(p_mModelCom);

	p_mMoveCom = new Com_PlayerMove();
	p_mMoveCom->SetMoveSpeed(10.0f);
	AddComponent(p_mMoveCom);

	p_mColliderCom = new Com_BoxCollider();
	p_mColliderCom->SetSize(1.0f, 2.0f, 1.0f);
	p_mColliderCom->SetCenter(0.0f, 1.0f, 0.0f);
	p_mColliderCom->bMovable = true;
	p_mColliderCom->Push(0);
	AddComponent(p_mColliderCom);

}

OBJ_Player::OBJ_Player(const char* _name)
	: OBJ_Player()
{
	sObjectName = _name;
}

void OBJ_Player::Update()
{
	if (bActive)
	{
		// 初期フレームのみ待機モーションの再生を開始
		if (bInit)
		{
			p_mModelCom->PlayAnimation("Idle", true);
			p_mModelCom->SetCurrentKeyFrame(0);
			bInit = false;
		}

		// 入力の受付(移動)
		Vector2 MoveVelo;
		if (Controller_Input::GetIsGamePadConnect(0))
		{
			MoveVelo.x = Controller_Input::GetLeftStick(0).x;
			MoveVelo.y = Controller_Input::GetLeftStick(0).y;
		}
		if (MoveVelo.x == 0 && MoveVelo.y == 0)
		{
			if (mState == PlayerState::Run)
			{
				fIdleCnt += Time->GetDeltaTime();
				if (fIdleCnt > fIdleDuration)
				{
					fIdleCnt = 0.0f;
					p_mModelCom->BlendAnimation("Idle", true);
					p_mModelCom->SetCurrentKeyFrame(0);
					mState = PlayerState::Idle;
				}
			}
		}
		else
		{
			if (mState == PlayerState::Idle)
			{
				p_mModelCom->BlendAnimation("Run", true);
				p_mModelCom->SetCurrentKeyFrame(0);
				mState = PlayerState::Run;
			}
		}

		// 入力受付(カメラ)
		p_mCameraCom->SetAngle(p_mCameraCom->GetAngle() + Controller_Input::GetRightStick(0).x);
		// 誤操作を防ぐため入力の受け付ける値を大きくする
		if (abs(Controller_Input::GetRightStick(0).y) > 0.5f)
		{
			p_mCameraCom->SetHeight(p_mCameraCom->GetHeight() + -(Controller_Input::GetRightStick(0).y * 0.2f));
		}


		// コア更新部
		switch (mState)
		{
		case OBJ_Player::PlayerState::Idle:
			break;
		case OBJ_Player::PlayerState::Run:
			/*if (p_mModelCom->GetState() == Com_SkeletalMesh::AnimationState::Play)
			{
				p_mModelCom->BlendAnimation("Run", true);
			}*/

			p_mMoveCom->Move(
				MoveVelo.x,
				MoveVelo.y);
			break;
		default:
			break;
		}
	}
}
