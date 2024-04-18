#pragma once
#include <SimpleMath.h>
// �}�e���A��
struct MATERIAL
{
	// �g�U����
	DirectX::SimpleMath::Color AlbedoColor;
	// ���Ɣ�������
	DirectX::SimpleMath::Color	 Emission;
	// �e�N�X�`���̗L����
	BOOL		TextureEnable;
	// �����x
	float		Metalic;
	// �e��
	float		Roughness;
	// �X�y�L�������x
	float		Shinness;

	MATERIAL()
	{
		AlbedoColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		Emission = DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.0f);
		Metalic = 1.0f;
		TextureEnable = true;
		Roughness = 0.0f;
		Shinness = 0.0f;
	}
};

struct PBR_MATERIAL
{
	// �g�U���ˌ�
	DirectX::SimpleMath::Color AlbedoColor;
	// �G�~�b�V����
	DirectX::SimpleMath::Color Emission;
	// �}�e���A��ID
	unsigned int MaterialID;
	// ���^���b�N
	float Metalic;
	// �e��
	float Roughness;
	// �}�C�N���t�@�Z�b�g
	float MicroFacet;
	// �X�y�L�������x
	float Shinness;
	// �@���}�b�v���x
	float NormalMap_Intensity;
	// �_�~�[
	float pad[2];

	PBR_MATERIAL()
	{
		AlbedoColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
		Emission = DirectX::SimpleMath::Color(0.0f, 0.0f, 0.0f, 0.0f);
		Metalic = 0.04f;
		Roughness = 0.5f;
		MicroFacet = 0.75f;
		MaterialID = 0;
		Shinness = 1.0f;
		NormalMap_Intensity = 0.0f;
		pad[0] = 0.0f;
		pad[1] = 0.0f;
	}
};




// �萔�o�b�t�@�p�\����
struct MatrixBufferData
{
	// ���[���h�ϊ��s��
	DirectX::SimpleMath::Matrix World;
	// �r���[�ϊ��s��
	DirectX::SimpleMath::Matrix View;
	// �v���W�F�N�V�����ϊ��s��
	DirectX::SimpleMath::Matrix Projection;
	// �X�P�[���f�[�^
	DirectX::SimpleMath::Vector4 Scale;
	// �J�����̃|�W�V�����f�[�^
	DirectX::SimpleMath::Vector4 CameraPos;
};