#pragma once
#include <d3d11.h>

// ID3D11ShaderResourceViewのカスタムクラス
class ShaderResourceView
{
protected:
	// テクスチャの実態
	ID3D11ShaderResourceView* p_mSRV{};

public:
	ShaderResourceView();
	// テクスチャを返す
	ID3D11ShaderResourceView* GetSRV() const;
	// テクスチャのセット
	void SetSRV(ID3D11ShaderResourceView* _pSRV);
	// テクスチャの作成
	virtual bool Create(const char* _FileName);

	virtual void Uninit();
};

// 画面から作る用
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

// レンダーターゲット
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

// 深度テクスチャ
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
