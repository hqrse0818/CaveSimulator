#include "Com_BillBoard.h"
#include "../DirectX/renderer.h"
#include "../GameObject/GameObject.h"

ID3D11Buffer* Com_BillBoard::m_VertexBuffer = nullptr;
Com_Camera* Com_BillBoard::p_TargetCamera = nullptr;
bool Com_BillBoard::bUpdated = false;
DirectX::SimpleMath::Matrix Com_BillBoard::mViewMtx{};
DirectX::SimpleMath::Matrix Com_BillBoard::mInvertViewMtx{};


using namespace DirectX::SimpleMath;

void Com_BillBoard::InitMain()
{
	Vector3 Normal = Vector3(0.0f, 1.0f, 0.0f);
	Vector4 Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	
	VERTEX_BILLBOARD vertex[BillBoardVertexNum];
	vertex[0].Position = Vector3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = Normal;
	vertex[0].TexCoord = Vector2(0.0f, 0.0f);
	vertex[0].Diffuse = Diffuse;

	vertex[1].Position = Vector3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = Normal;
	vertex[1].TexCoord = Vector2(1.0f, 0.0f);
	vertex[1].Diffuse = Diffuse;

	vertex[2].Position = Vector3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = Normal;
	vertex[2].TexCoord = Vector2(0.0f, 1.0f);
	vertex[2].Diffuse = Diffuse;

	vertex[3].Position = Vector3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = Normal;
	vertex[3].TexCoord = Vector2(1.0f, 1.0f);
	vertex[3].Diffuse = Diffuse;

	HRESULT hr = CreateVertexBufferWrite(Renderer::GetDevice(), sizeof(VERTEX_BILLBOARD), BillBoardVertexNum, vertex, &m_VertexBuffer);

	if (FAILED(hr))
	{
		DEBUGLOG("頂点バッファ作成に失敗");
	}
}

void Com_BillBoard::UninitMain()
{
	SAFE_RELEASE(m_VertexBuffer);
}

void Com_BillBoard::Update()
{
	if (!bUpdated)
	{
		// カメラのビュー行列
		mViewMtx = p_TargetCamera->GetViewMatrix();
		// カメラのビュー行列の逆行列
		mInvertViewMtx = mViewMtx.Invert();
		mInvertViewMtx._41 = 0.0f;
		mInvertViewMtx._42 = 0.0f;
		mInvertViewMtx._43 = 0.0f;

		bUpdated = true;
	}
}

void Com_BillBoard::Draw()
{
	// 頂点バッファの更新
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(hr))
	{
		VERTEX_BILLBOARD* vertex = (VERTEX_BILLBOARD*)msr.pData;
		float Half = 0.5;
		Vector3 Normal = Vector3(0.0f, 1.0f, 0.0f);

		vertex[0].Position = Vector3(-mSize.x * Half, mSize.y *Half, p_mObject->p_mTransform->mPosition.z);
		vertex[0].Normal = Normal;
		vertex[0].Diffuse = mColor;
		vertex[0].TexCoord = Vector2(fCurrentX, fCurrentY);

		vertex[1].Position = Vector3(mSize.x * Half, mSize.y * Half, p_mObject->p_mTransform->mPosition.z);
		vertex[1].Normal = Normal;
		vertex[1].Diffuse = mColor;
		vertex[1].TexCoord = Vector2(fCurrentZ, fCurrentY);

		vertex[2].Position = Vector3(-mSize.x * Half, -mSize.y * Half, p_mObject->p_mTransform->mPosition.z);
		vertex[2].Normal = Normal;
		vertex[2].Diffuse = mColor;
		vertex[2].TexCoord = Vector2(fCurrentX, fCurrentW);

		vertex[3].Position = Vector3(mSize.x * Half, -mSize.y * Half , p_mObject->p_mTransform->mPosition.z);
		vertex[3].Normal = Normal;
		vertex[3].Diffuse = mColor;
		vertex[3].TexCoord = Vector2(fCurrentZ, fCurrentW);

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	}

	Matrix world, scale, trans;

	scale = Matrix::CreateScale(p_mObject->p_mTransform->mScale.x);
	Vector3 pos = p_mObject->p_mTransform->mPosition;
	trans = Matrix::CreateTranslation(pos.x, pos.y, pos.z);
	world = scale * mInvertViewMtx * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファの設定
	UINT stride = sizeof(VERTEX_BILLBOARD);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IAGetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャの設定
	ID3D11ShaderResourceView* pSRV = m_Texture->GetSRV();

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &pSRV);

	Renderer::SetTopologyTriangleStrip();

	Renderer::Render(BillBoardVertexNum, 0);
	bUpdated = false;
}

void Com_BillBoard::SetTexture(const char* _FileName)
{
	if (m_Texture == nullptr)
	{
		m_Texture = new ShaderResourceView();
	}
	m_Texture->Create(_FileName);
}

void Com_BillBoard::SetTexture(ShaderResourceView* _psrv)
{
	m_Texture = _psrv;
}

void Com_BillBoard::SetCurrent(int _val)
{
	iCurrent = _val;

	// テクスチャ座標を算出する

	int SeparateNum = iSeparateU * iSeparateV;
	if (iCurrent > SeparateNum)
	{
		iCurrent = iCurrent % SeparateNum;
	}

	int u = iCurrent % iSeparateU;
	float offsetU = 1.0f / iSeparateU;
	fCurrentX = offsetU * (u - 1);
	fCurrentZ = offsetU * u;

	float offsetV = 1.0f / iSeparateV;
	int v = 1;
	int num = iCurrent;
	while (true)
	{
		if (num > iSeparateU)
		{
			v++;
			num = num - iSeparateU;
		}
		else
		{
			break;
		}
	}
	fCurrentY = offsetV * (v - 1);
	fCurrentW = offsetV * v;
}
