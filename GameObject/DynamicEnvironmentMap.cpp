#include "DynamicEnvironmentMap.h"
#include "GameObject.h"
#include "../Component/Com_Sprite.h"
#include "../DirectX/ShaderResourceView.h"
#include "../DirectX/renderer.h"

DynamicEnvironmentMap::DynamicEnvironmentMap(const char* _ObjectName, float _SpriteWidth, float _SpriteHeight)
{
	p_mSpriteObj = new GameObject(_ObjectName);
	p_mSpriteBuf = new Com_Sprite();
	p_mSpriteBuf->SetVertex("VS_SPRITE");
	p_mSpriteBuf->SetPixel("PS_DisneyPBR");
	p_mSpriteObj->AddComponent(p_mSpriteBuf);
	p_mSpriteBuf->SetSize(_SpriteWidth, _SpriteHeight);
	p_mSpriteObj->SetPosition(_SpriteWidth / 2, _SpriteHeight / 2);

	p_mRTVEnv = new RenderTarget();
	p_mRTVNormal = new RenderTarget();
	p_mRTVAlbedo = new RenderTarget();
	p_mRTVWorld = new RenderTarget();
	p_mRTVMaterial = new RenderTarget();
	p_mDSV = new DepthStencil();

}

void DynamicEnvironmentMap::InitObject()
{
	p_mSpriteObj->InitBase();
}

HRESULT DynamicEnvironmentMap::InitRender(UINT _width, UINT _height, bool _useStencil)
{
	HRESULT hr;

	// レンダーターゲットの作成
	hr = p_mRTVEnv->Create(DXGI_FORMAT_R32G32B32A32_FLOAT, _width, _height);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("環境マップレンダーターゲットの作成に失敗");
		return hr;
	}
	hr = p_mRTVNormal->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, _width, _height);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("環境マップレンダーターゲットの作成に失敗");
		return hr;
	}
	p_mRTVAlbedo->Create(DXGI_FORMAT_R8G8B8A8_UNORM, _width, _height);
	p_mRTVWorld->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, _width, _height);
	p_mRTVMaterial->Create(DXGI_FORMAT_R16_UNORM, _width, _height);
	// デプスステンシルビューの作成
	p_mDSV->Create(_width, _height, _useStencil);

	return hr;
}

void DynamicEnvironmentMap::SetPosition(float _x, float _y, float _z)
{
	p_mSpriteObj->SetPosition(_x, _y, _z);
}

void DynamicEnvironmentMap::SetGBuffer()
{
	RenderTarget* GBuffer[] =
	{
		p_mRTVNormal,
		p_mRTVAlbedo,
		p_mRTVWorld,
		p_mRTVMaterial,
	};
	// GBufferのクリア
	GBuffer[0]->Clear();
	GBuffer[1]->Clear();
	GBuffer[2]->Clear();
	GBuffer[3]->Clear();
	// デプスステンシルビューのクリア
	p_mDSV->Clear();

	Renderer::SetRenderTargets(4, GBuffer, p_mDSV);
}

void DynamicEnvironmentMap::ExcuteRendering(ID3D11ShaderResourceView* _CubeMap)
{
	p_mRTVEnv->Clear();
	Renderer::SetRenderTargets(1, &p_mRTVEnv, nullptr);
	p_mSpriteBuf->GetPS()->SetTexture(1, p_mRTVNormal);
	p_mSpriteBuf->GetPS()->SetTexture(2, p_mRTVAlbedo);
	p_mSpriteBuf->GetPS()->SetTexture(3, p_mRTVWorld);
	p_mSpriteBuf->GetPS()->SetTexture(4, p_mRTVMaterial);
	p_mSpriteBuf->GetPS()->SetTexture(5, _CubeMap);
	p_mSpriteObj->Draw();
}

void DynamicEnvironmentMap::Uninit()
{
	p_mSpriteObj->Uninit();
	SAFE_NULL(p_mSpriteObj);
	p_mDSV->Uninit();
	SAFE_NULL(p_mDSV);
	p_mRTVEnv->Uninit();
	SAFE_NULL(p_mRTVEnv);
	p_mRTVMaterial->Uninit();
	SAFE_NULL(p_mRTVMaterial);
	p_mRTVWorld->Uninit();
	SAFE_NULL(p_mRTVWorld);
	p_mRTVAlbedo->Uninit();
	SAFE_NULL(p_mRTVAlbedo);
	p_mRTVNormal->Uninit();
	SAFE_NULL(p_mRTVNormal)
}

RenderTarget* DynamicEnvironmentMap::GetDefferedTexture()
{
	return p_mRTVEnv;
}

RenderTarget* DynamicEnvironmentMap::GetNormalRTV()
{
	return p_mRTVNormal;
}

RenderTarget* DynamicEnvironmentMap::GetAlbedoRTV()
{
	return p_mRTVAlbedo;
}

RenderTarget* DynamicEnvironmentMap::GetWorldRTV()
{
	return p_mRTVWorld;
}

RenderTarget* DynamicEnvironmentMap::GetMaterialRTV()
{
	return p_mRTVMaterial;
}
