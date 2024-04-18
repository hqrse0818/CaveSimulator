#include "ShaderResourceView.h"
#include "../System/TextureCreate.h"
#include "../Utility.h"
#include <iostream>
#include "../Debug.h"
#include "renderer.h"

using namespace std;

ShaderResourceView::ShaderResourceView()
	: p_mSRV(nullptr)
{
}

ID3D11ShaderResourceView* ShaderResourceView::GetSRV() const
{
	return p_mSRV;
}

void ShaderResourceView::SetSRV(ID3D11ShaderResourceView* _pSRV)
{
	p_mSRV = _pSRV;
}

bool ShaderResourceView::Create(const char* _FileName)
{
	TextureCreate::CreateTexture(_FileName, &p_mSRV);
	if (p_mSRV == nullptr)
	{
		NEWDEBUGLOG(_FileName << "画像ファイルが見つかりません");
		return false;
	}
	return true;
}

void ShaderResourceView::Uninit()
{
	SAFE_RELEASE(p_mSRV);
}

// テクスチャ2D

Texture2D::Texture2D()
	: p_mTex(nullptr), mWidth(0), mHeight(0)
{
}

void Texture2D::Uninit()
{
	ShaderResourceView::Uninit();
	SAFE_RELEASE(p_mTex);
}

UINT Texture2D::GetWidth() const
{
	return mWidth;
}

UINT Texture2D::GetHeight() const
{
	return mHeight;
}

HRESULT Texture2D::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	return CreateResource(desc, pData);
}
 
HRESULT Texture2D::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr)
{
	HRESULT hr = E_FAIL;

	//テクスチャの作成
	/* D3D11_SUBRESOURCE_DATA テクスチャやバッファの初期データを指定するための構造体
	* pSysMem初期データのポインタ
	* SysMemPitch テクスチャの行ごとのバイト数テクスチャの横幅以上
	* SysMemSlicePitch テクスチャ全体のバイト数 
	*/
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = pData;
	sd.SysMemPitch = desc.Width * sizeof(uint32_t);
	hr = Renderer::GetDevice()->CreateTexture2D(&desc, pData ? &sd : nullptr, &p_mTex);
	if (FAILED(hr))
		return hr;

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	switch (desc.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS:
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT; break;
	default:
		srvDesc.Format = desc.Format;
		break;
	}

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// 生成
	hr = Renderer::GetDevice()->CreateShaderResourceView(p_mTex, &srvDesc, &p_mSRV);
	if (SUCCEEDED(hr))
	{
		mWidth = desc.Width;
		mHeight = desc.Height;
	}

	return hr;
}


D3D11_TEXTURE2D_DESC Texture2D::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
{
	// テクスチャデスクの設定
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	// シェーダーリソースとして扱う
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;

	DXGI_SWAP_CHAIN_DESC swapdesc{};
	Renderer::GetSwapChain()->GetDesc(&swapdesc);
	desc.SampleDesc = swapdesc.SampleDesc;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	return desc;
}


// レンダーターゲット

RenderTarget::RenderTarget()
	: p_mRTV(nullptr)
{
}

void RenderTarget::Uninit()
{
	Texture2D::Uninit();
	SAFE_RELEASE(p_mRTV);
}

void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}

void RenderTarget::Clear(const float* color)
{
	Renderer::GetDeviceContext()->ClearRenderTargetView(p_mRTV, color);
}

HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	// テクスチャデスクの作成
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	// Texture2D, SRV, RTVの作成
	return CreateResource(desc);
}

HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;

	// バックバッファのポインタを取得
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = Renderer::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&p_mTex);
	if (FAILED(hr))
		return hr;

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = Renderer::GetDevice()->CreateRenderTargetView(p_mTex, NULL, &p_mRTV);
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;
		p_mTex->GetDesc(&desc);
		mWidth = desc.Width;
		mHeight = desc.Height;
	}

	return hr;
}

ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return p_mRTV;
}

HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// テクスチャリソースの作成
	HRESULT hr = Texture2D::CreateResource(desc, nullptr);
	if (FAILED(hr))
		return hr;

	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = desc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// 生成
	return Renderer::GetDevice()->CreateRenderTargetView(p_mTex, NULL, &p_mRTV);
}

DepthStencil::DepthStencil()
	: p_mDSV(nullptr)
{
}

void DepthStencil::Uninit()
{
	Texture2D::Uninit();
	SAFE_RELEASE(p_mDSV);
}

void DepthStencil::Clear()
{
	Renderer::GetDeviceContext()->ClearDepthStencilView(p_mDSV, D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/, 1.0f, 0);
}

HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);

	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	//desc.Format = DXGI_FORMAT_D16_UNORM;
	return CreateResource(desc);
}

ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return p_mDSV;
}

HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// ステンシル使用判定
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

	// リソース生成
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = Texture2D::CreateResource(desc, nullptr);
	if (FAILED(hr))
		return hr;

	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;

	// 生成
	return Renderer::GetDevice()->CreateDepthStencilView(p_mTex, &dsvDesc, &p_mDSV);
}
