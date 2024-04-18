#pragma once
#include "../Utility.h"
#include "../DirectX/ShaderResourceView.h"

class GameObject;
class Com_Sprite;

constexpr int CubeMapArraySize = 6;

class CubeMapGenerator
{
public:
	ID3D11ShaderResourceView* GetCubeMap() const
	{
		return pCubeMap;
	};

	// RTVArray�ƃX�v���C�g�I�u�W�F�N�g�̏�����
	void Init(UINT _width, UINT _height);
	// ���
	void Uninit();
	// �쐬
	void Generate(RenderTarget** _texture);

private:
	// �����_�[�^�[�Q�b�g�̕��ƍ���
	UINT mWidth, mHeight;

	GameObject* pObject;
	Com_Sprite* pSprite;

	// �����_�����O���ʂ̊i�[��
	ID3D11ShaderResourceView* pCubeMap;
	// array��RTV
	ID3D11RenderTargetView* pRTVArray;
	// �e�N�X�`���f�X�N�̐ݒ�
	D3D11_TEXTURE2D_DESC mDesc;
};

