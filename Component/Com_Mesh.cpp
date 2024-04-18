#include "Com_Mesh.h"
#include "../Debug.h"
#include "../DirectX/renderer.h"

using namespace DirectX::SimpleMath;


Com_Mesh::Com_Mesh()
{
	mScale.x = 1.0f;
	mScale.y = 1.0f;
	mScale.z = 1.0f;
	mScale.w = 1.0f;
	// 定数バッファを作成
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vector4);
	bd.CPUAccessFlags = 0;

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, NULL, &p_mScaleBuffer);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("スケール用バッファ作成失敗");
	}
	else
	{
		// スケールバッファの更新
		Renderer::GetDeviceContext()->UpdateSubresource(p_mScaleBuffer, 0, nullptr, &mScale, 0, 0);
	}
}

void Com_Mesh::Bind()
{
	Com_Visual::Bind();
	if (bApplyNormalMap)
	{
		ID3D11ShaderResourceView* NormalSRV = p_mNormalMap->GetSRV();
		Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &NormalSRV);
	}
}

void Com_Mesh::Uninit()
{
	if (p_mScaleBuffer)
	{
		p_mScaleBuffer->Release();
		p_mScaleBuffer = nullptr;
	}
	if (bApplyNormalMap)
	{
		p_mNormalMap->Uninit();
		SAFE_NULL(p_mNormalMap);
	}
}

void Com_Mesh::SetNormalMap(const char* _FileName)
{
	bApplyNormalMap = true;
	p_mNormalMap = new ShaderResourceView();
	p_mNormalMap->Create(_FileName);
}
