#pragma once
#include <SimpleMath.h>

class DirectionalLight
{
public:
	BOOL Enable;
	DirectX::SimpleMath::Vector3 mPosition;
	DirectX::SimpleMath::Vector4	Direction;
	// x,y,z �J���[ w ���C�g�̋���
	DirectX::SimpleMath::Color	Diffuse;
	// ����
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Matrix WorldMatrix;
	DirectX::SimpleMath::Matrix ViewMatrix;
	DirectX::SimpleMath::Matrix ProjectionMatrix;
	DirectX::SimpleMath::Color BaseColor;
	int PointLightNum;
	int SpotLightNum;
	float dummy[2];
	
public:

	DirectX::SimpleMath::Matrix WorldIdentity();
	// ���[���h�ϊ��s��̌v�Z
	DirectX::SimpleMath::Matrix CalcWorldMatrix();
	// �r���[�ϊ��s��̌v�Z
	DirectX::SimpleMath::Matrix CalcView();
	// �v���W�F�N�V�����ϊ��s��쐬
	DirectX::SimpleMath::Matrix CalcProjection(float fovx, float fovy, float clipnear, float clipfar);
};

class SpotLight
{
public:
	DirectX::SimpleMath::Color Diffuse;
	DirectX::SimpleMath::Vector3 Position;
	float range;
	DirectX::SimpleMath::Vector3 Direction;
	float angle;
};

class PointLight
{
public:
	// x,y,z �ʒu��� w �e���͈�
	DirectX::SimpleMath::Vector4 Position;
	// x,y,z �J���[ w ���C�g�̋���
	DirectX::SimpleMath::Color Diffuse;
};


// �����|�C���g���C�g
class CustomPointLight
{
private:
	// �z��Ɋi�[������ۂ̃f�[�^
	PointLight* pLight;

public:
	// �͈͊֘A
	float mInitRange = 0;
	float mMaxRange = 0;
	float mMinRange = 0;
	float mGrowSpeed = 0;

	// �|�W�V�����֘A
	float mAngleXZ = 0;
	float mAngleY = 0;
	float mAngleSpeedXZ = 0;
	float mAngleSpeedY = 0;
	float mLength = 0;

	bool bReturn = false;
	DirectX::SimpleMath::Vector3  AxisPos = DirectX::SimpleMath::Vector3(0.0f,0.0f, 0.0f);

public:
	CustomPointLight();
	void SetColor(float r, float g, float b, float a);
	void InitPointLight();
	void UpdatePointLight();

	PointLight* GetLight()
	{
		return pLight;
	}
};