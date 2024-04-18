#include "Com_Transform.h"
#include "../Utility.h"

using namespace DirectX::SimpleMath;

void Com_Transform::Update()
{
	DirectX::SimpleMath::Matrix MyRotation;
	// 自身の回転角度から前向きベクトルを得るための行列を生成
	MyRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(mRotation.y + mForwardTuneValue.y, mRotation.x + mForwardTuneValue.x, mRotation.z + mForwardTuneValue.z);

	// 前向きベクトル情報を格納
	mForward.x = MyRotation._31;
	mForward.y = MyRotation._32;
	mForward.z = MyRotation._33;

	mForward.Normalize();

	// 自身の回転角度から左向きベクトルを得るための行列を生成
	MyRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(mRotation.y + mForwardTuneValue.y - (float)PIE / 2, mRotation.x + mForwardTuneValue.x, mRotation.z + mForwardTuneValue.z);

	// 右向きベクトル情報を格納
	mRight.x = MyRotation._31;
	mRight.y = MyRotation._32;
	mRight.z = MyRotation._33;
	mRight.Normalize();
}

void Com_Transform::PreDraw()
{
	
}
