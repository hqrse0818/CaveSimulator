#pragma once
#include "../Format.h"
#include <vector>

constexpr unsigned int MaxPointLightNum = 1024;
constexpr unsigned int MaxSpotLightNum = 1000;

struct PointLights
{
	PointLight lights[MaxPointLightNum];
};

struct SpotLights
{
	SpotLight lights[MaxSpotLightNum];
};


class LightPool
{
private:
	// ���s����
	static DirectionalLight* pDirectionLight;
	// �|�C���g���C�g�̃��X�g
	static std::vector<PointLight*> mPointLightList;
	// �X�|�b�g���C�g�̃��X�g
	static std::vector<SpotLight*> mSpotLightList;

	static ID3D11Buffer* p_mDirectionalLightBuf;
	static ID3D11Buffer* p_mPointLightsBuf;
	static ID3D11Buffer* p_mSpotLightsBuf;

public:
	// �|�C���g���C�g�̒萔�o�b�t�@�쐬
	static HRESULT CreatePointLightBuffer();
	// �|�C���g���C�g�̒ǉ�
	static void Push(PointLight* _light);
	// �|�C���g���C�g�X�V
	static void UpdatePointLights();
	// �|�C���g���C�g�̒萔�o�b�t�@�o�^
	static void SetPointLightsBuffer(int _slot);
	// �|�C���g���C�g�݂̂̍폜
	static void ReleasePointLight();

	// �X�|�b�g���C�g�̒萔�o�b�t�@�쐬
	static HRESULT CreateSpotLightBuffer();
	// �X�|�b�g���C�g�̒ǉ�
	static void Push(SpotLight* _light);
	// �X�|�b�g���C�g�X�V
	static void UpdateSpotLights();
	// �X�|�b�g���C�g�̒萔�o�b�t�@�o�^
	static void SetSpotLightBuffer(int _slot);
	// �X�|�b�g���C�g�̂ݍ폜
	static void ReleaseSpotLight();

	static void UpdateAllLights();
	// ���s�����쐬
	static HRESULT CreateDirectionalLight();
	// ���s�����擾
	static DirectionalLight* GetDirectioLight();
	// ���s�����X�V
	static void UpdateDirectionalLight();
	// ���s�����̒萔�o�b�t�@�o�^
	static void SetDirectionalLightBuffer();

	// ���
	static void Uninit();

	static void ShowLightGui();

	static void SaveLight(const char* _FileName);
};

