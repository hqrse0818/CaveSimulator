#include "Com_Camera.h"
#include "../DirectX/renderer.h"
#include "../GameObject/GameObject.h"
#include "../main.h"

using namespace DirectX::SimpleMath;


void Com_Camera::SetTarget(GameObject* _targetpos)
{
	p_mTarget = _targetpos;
	SetUseTarget(true);
}

void Com_Camera::Init()
{
	
}

void Com_Camera::Update()
{
	if (p_mTarget != nullptr)
	{
		mTargetPosition = p_mTarget->p_mTransform->mPosition;
		mTargetPosition *= mTargetCorrect;
	}
	// �r���[�ϊ��s��쐬
	mViewMatrix = DirectX::XMMatrixLookAtLH(p_mObject->p_mTransform->mPosition, mTargetPosition, mUp);
	// ��ʂ̔䗦�̐ݒ�
	float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);

	// �v���W�F�N�V�����s��̌v�Z
	mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fFov), aspect, fNear, fFar);
}

void Com_Camera::Draw()
{
	// �r���[�s����Z�b�g
	Renderer::SetViewMatrix(&mViewMatrix);
	Renderer::SetProjectionMatrix(&mProjectionMatrix);
}
