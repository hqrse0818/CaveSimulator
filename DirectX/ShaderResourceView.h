#pragma once
#include <d3d11.h>

// ID3D11ShaderResourceView�̃J�X�^���N���X
class ShaderResourceView
{
protected:
	// �e�N�X�`���̎���
	ID3D11ShaderResourceView* p_mSRV{};

public:
	ShaderResourceView();
	// �e�N�X�`����Ԃ�
	ID3D11ShaderResourceView* GetSRV() const;
	// �e�N�X�`���̃Z�b�g
	void SetSRV(ID3D11ShaderResourceView* _pSRV);
	// �e�N�X�`���̍쐬
	virtual bool Create(const char* _FileName);

	virtual void Uninit();
};

// ��ʂ�����p
class Texture2D
	: public ShaderResourceView
{
protected:
	UINT mWidth;
	UINT mHeight;
	ID3D11Texture2D* p_mTex{};

public:
	Texture2D();
	void Uninit();

	UINT GetWidth()const;
	UINT GetHeight()const;
	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData);
protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData);
	D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height);
};

// �����_�[�^�[�Q�b�g
class RenderTarget
	: public Texture2D
{
private:
	ID3D11RenderTargetView* p_mRTV;

public:
	RenderTarget();
	void Uninit();

	void Clear();
	void Clear(const float* color);

	HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
	HRESULT CreateFromScreen();
	ID3D11RenderTargetView* GetView() const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);
};

// �[�x�e�N�X�`��
class DepthStencil
	: public Texture2D
{
private:
	ID3D11DepthStencilView* p_mDSV;

public:
	DepthStencil();
	void Uninit();

	void Clear();
	HRESULT Create(UINT width, UINT height, bool useStencil);
	ID3D11DepthStencilView* GetView()const;

protected:
	virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);
};
