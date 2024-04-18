#include "CubeMapGenerator.h"

#include "renderer.h"
#include "../main.h"

#include "../GameObject/GameObject.h"
#include "../Component/Com_Sprite.h"

using namespace DirectX::SimpleMath;

void CubeMapGenerator::Init(UINT _width, UINT _height)
{
	mWidth = _width;
	mHeight = _height;

	float Half = 0.5f;

	// 真ん中にスプライトを描画するためのオブジェクト
	pObject = new GameObject("EnvObj");
	pSprite = new Com_Sprite();
	pSprite->SetSize(_width, _height);
	pSprite->SetVertex("VS_SPRITE");
	pSprite->SetPixel("PS_SPRITE");
	pObject->AddComponent(pSprite);
	pObject->SetPosition(_width * Half, _height * Half);
	pObject->InitBase();

	// キューブマップとレンダーターゲットビューの作成
	if (pCubeMap == nullptr && pRTVArray == nullptr)
	{
		// テクスチャリソース
		mDesc = {};
		mDesc.Width = _width;
		mDesc.Height = _height;
		mDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// 6枚の画像を持てるようにする(texureArray)
		mDesc.ArraySize = CubeMapArraySize;
		mDesc.MiscFlags =
			D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		mDesc.MipLevels = 0;
		mDesc.CPUAccessFlags = 0;
		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;

		ID3D11Texture2D* tex = nullptr;
		HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&mDesc, nullptr, &tex);
		if (FAILED(hr))
		{
			NEWDEBUGLOG("テクスチャリソース作成失敗");
		}
		else
		{
			tex->GetDesc(&mDesc);

			// RTV作成
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = mDesc.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;	// テクスチャ配列
			rtvDesc.Texture2DArray.ArraySize = mDesc.ArraySize;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.MipSlice = 0;
			hr = Renderer::GetDevice()->CreateRenderTargetView(tex, &rtvDesc, &pRTVArray);
			if (FAILED(hr))
			{
				NEWDEBUGLOG("環境マッピングRTV作成失敗");
			}


			// SRV作成
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = mDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = mDesc.MipLevels;
			hr = Renderer::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &pCubeMap);
			if (FAILED(hr))
			{
				NEWDEBUGLOG("環境マッピングSRV作成失敗");
			}
			if (tex != nullptr)
				tex->Release();
		}
	}
}

void CubeMapGenerator::Uninit()
{
	pObject->Uninit();
	SAFE_NULL(pObject);
	SAFE_RELEASE(pCubeMap);
	SAFE_RELEASE(pRTVArray);
}

void CubeMapGenerator::Generate(RenderTarget** _texture)
{
	// テクスチャリソースを取得
	ID3D11Resource* cubeMapResource = nullptr;
	pRTVArray->GetResource(&cubeMapResource);
	float ClearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < CubeMapArraySize; i++)
	{
		ID3D11RenderTargetView* rtv = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC rtvDsc = {};
		rtvDsc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtvDsc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDsc.Texture2DArray.ArraySize = 1;
		rtvDsc.Texture2DArray.FirstArraySlice = i;
		rtvDsc.Texture2DArray.MipSlice = 0;
		// レンダーターゲット作成
		HRESULT hr = Renderer::GetDevice()->CreateRenderTargetView(cubeMapResource, &rtvDsc, &rtv);
		if (FAILED(hr))
		{
			NEWDEBUGLOG("動的RTV作成失敗" << i);
		}
		else
		{
			// レンダーターゲットクリア
			Renderer::GetDeviceContext()->ClearRenderTargetView(rtv, ClearColor);
			// レンダーターゲットを設定
			Renderer::SetRenderTargets(1, &rtv, nullptr, mWidth, mHeight);
			// 受け取ったテクスチャを設定
			pSprite->SetTexture(_texture[i]);
			// スプライトとして描画
			pObject->Draw();
			// ミップマップの生成
			Renderer::GetDeviceContext()->GenerateMips(pCubeMap);

			SAFE_RELEASE(rtv);
		}
	}
}
