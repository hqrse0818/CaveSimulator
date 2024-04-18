#include "Com_SpringLight.h"
#include "../System/LightPool.h"
#include "../System/Time.h"

using namespace DirectX::SimpleMath;



Com_SpringLight::Com_SpringLight()
{
	p_mLight = new PointLight();
	p_mLight->Position = Vector4(0.0f, 0.0f, 0.0f, 50.0f);
	p_mLight->Diffuse = Color(1.0f, 1.0f, 1.0f, 10.0f);
	LightPool::Push(p_mLight);
}

void Com_SpringLight::SpringStart()
{
	mVelocity = Vector3::Zero;
	mGravity.x = mGravity.x * mMass;
	mGravity.y = mGravity.y * mMass;
	mGravity.z = mGravity.z * mMass;
	p_mLight->Position.x = mInitPos.x;
	p_mLight->Position.y = mInitPos.y;
	p_mLight->Position.z = mInitPos.z;
	mState = SpringState::Do;
}

void Com_SpringLight::SpringUpdate()
{
	Vector3 tension;
	tension.x = -mSpringCoefficient * (p_mLight->Position.x - mSpringLength);
	tension.y = -mSpringCoefficient * (p_mLight->Position.y - mSpringLength);
	tension.z = -mSpringCoefficient * (p_mLight->Position.z - mSpringLength);
	// 加速度を求める
	Vector3 Acceleration = (((mGravity + tension) + (-mVelocity * mAtt)) / mMass) * Time->GetDeltaTime();
	// 速度に加速度を加算
	mVelocity += Acceleration;

	// 速度から座標の計算
	p_mLight->Position.x += mVelocity.x;
	p_mLight->Position.y += mVelocity.y;
	p_mLight->Position.z += mVelocity.z;

	if (fabs(Acceleration.x < AccelaratePermissionVal) && fabs(Acceleration.y < AccelaratePermissionVal) && fabs(Acceleration.z < AccelaratePermissionVal))
	{
		mState = SpringState::End;
	}
}

void Com_SpringLight::Init()
{
	mState = SpringState::Start;
}

void Com_SpringLight::Update()
{
	switch (mState)
	{
	case Com_SpringLight::SpringState::Start:
		SpringStart();
		break;
	case Com_SpringLight::SpringState::Do:
		SpringUpdate();
		break;
	case Com_SpringLight::SpringState::End:
		// 初期化
		mState = SpringState::Start;
		break;
	}
}
