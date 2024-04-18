#include "Com_AngleCamera.h"
#include "../GameObject/GameObject.h"
#include "../Utility.h"

void Com_AngleCamera::Update()
{
	// CurrentAngle‚©‚çŒ»Ý‚ÌˆÊ’u‚ð‹‚ß‚é

	float cosf_a_b = cosf(DirectX::XMConvertToRadians(fCurrentAngle));
	float cosf_h_b = cosf(DirectX::XMConvertToRadians(fHeight));
	float sinf_a_b = sin(DirectX::XMConvertToRadians(fCurrentAngle));
	float sinf_h_b = sin(DirectX::XMConvertToRadians(fHeight));

	float x = cosf_h_b  * sinf_a_b;
	float z = cosf_h_b  * cosf_a_b;
	float y = sinf_h_b;
	p_mObject->p_mTransform->mPosition.x = x * fDistance;
	p_mObject->p_mTransform->mPosition.z = z * fDistance;
	p_mObject->p_mTransform->mPosition.y = y * fDistance;

	if (p_mTarget)
	{
		p_mObject->p_mTransform->mPosition.x += p_mTarget->p_mTransform->mPosition.x;
		p_mObject->p_mTransform->mPosition.z += p_mTarget->p_mTransform->mPosition.z;
		//p_mObject->p_mTransform->mPosition.y = p_mTarget->p_mTransform->mPosition.y;
	}
	else
	{
		p_mObject->p_mTransform->mPosition.x += mTargetPosition.x;
		p_mObject->p_mTransform->mPosition.z += mTargetPosition.z;
	}
	

	Com_Camera::Update();
}
