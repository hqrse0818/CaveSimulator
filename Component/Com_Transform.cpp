#include "Com_Transform.h"
#include "../Utility.h"

using namespace DirectX::SimpleMath;

void Com_Transform::Update()
{
	DirectX::SimpleMath::Matrix MyRotation;
	// ���g�̉�]�p�x����O�����x�N�g���𓾂邽�߂̍s��𐶐�
	MyRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(mRotation.y + mForwardTuneValue.y, mRotation.x + mForwardTuneValue.x, mRotation.z + mForwardTuneValue.z);

	// �O�����x�N�g�������i�[
	mForward.x = MyRotation._31;
	mForward.y = MyRotation._32;
	mForward.z = MyRotation._33;

	mForward.Normalize();

	// ���g�̉�]�p�x���獶�����x�N�g���𓾂邽�߂̍s��𐶐�
	MyRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(mRotation.y + mForwardTuneValue.y - (float)PIE / 2, mRotation.x + mForwardTuneValue.x, mRotation.z + mForwardTuneValue.z);

	// �E�����x�N�g�������i�[
	mRight.x = MyRotation._31;
	mRight.y = MyRotation._32;
	mRight.z = MyRotation._33;
	mRight.Normalize();
}

void Com_Transform::PreDraw()
{
	
}
