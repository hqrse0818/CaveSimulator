#include "Geometory.h"
#include "../Utility.h"
#include "renderer.h"
#include "../System/ShaderPool.h"

ID3D11Buffer* Geometory::pBoxBuffer;
ID3D11Buffer* Geometory::pLineBuffer;
ID3D11Buffer* Geometory::pSphereBuffer;
VertexShader* Geometory::pVS;
PixelShader* Geometory::pPS;

using namespace DirectX::SimpleMath;

void Geometory::Create()
{
	LINE vertex;
	vertex.vx[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertex.vx[0].Normal = Vector3(0.0f, 1.0, 0.0f);
	vertex.vx[0].Diffuse = Color(1.0f, 1.0f, 0.0f, 1.0f);
	vertex.vx[1].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertex.vx[1].Normal = Vector3(0.0f, 1.0, 0.0f);
	vertex.vx[1].Diffuse = Color(1.0f, 1.0f, 0.0f, 1.0f);

	HRESULT hr = CreateVertexBufferWrite(Renderer::GetDevice(), sizeof(VERTEX_GEOMETORY), 2, &vertex, &pLineBuffer);
}

void Geometory::Uninit()
{
	SAFE_RELEASE(pLineBuffer);
	SAFE_RELEASE(pSphereBuffer);
	SAFE_RELEASE(pBoxBuffer);
}

void Geometory::DrawBox(DirectX::SimpleMath::Vector3 _pos, DirectX::SimpleMath::Vector3 _size, DirectX::SimpleMath::Vector3 _rot, DirectX::SimpleMath::Color _color)
{
}

void Geometory::DrawLine(DirectX::SimpleMath::Vector3 _PosS, DirectX::SimpleMath::Color _ColS, DirectX::SimpleMath::Vector3 _PosE, DirectX::SimpleMath::Color _ColE)
{
	D3D11_MAPPED_SUBRESOURCE msd{};
	LINE* vx;
	HRESULT hr = Renderer::GetDeviceContext()->Map(pLineBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msd);
	if (SUCCEEDED(hr))
	{
		// 頂点データの更新
		vx = (LINE*)msd.pData;
		vx->vx[0].Position = _PosS;
		vx->vx[0].Diffuse = _ColS;
		vx->vx[0].Normal = Vector3(0.0f, 1.0f, 0.0f);
		vx->vx[1].Position = _PosE;
		vx->vx[1].Diffuse = _ColE;
		vx->vx[1].Normal = Vector3(0.0f, 1.0f, 0.0f);
		Renderer::GetDeviceContext()->Unmap(pLineBuffer, 0);

		Matrix mat = Matrix::Identity;
		Renderer::SetWorldMatrix(&mat);
		pVS->Bind();
		pPS->Bind();

		UINT stride = sizeof(VERTEX_GEOMETORY);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &pLineBuffer, &stride, &offset);
		Renderer::SetTopologyLineList();
		Renderer::GetDeviceContext()->Draw(2, 0);
	}
}

void Geometory::DrawSphere(DirectX::SimpleMath::Vector3 _Pos, float _radius, DirectX::SimpleMath::Color _color)
{
}

void Geometory::SetVertex(const char* _name)
{
	pVS = static_cast<VertexShader*>(ShaderPool::GetShader(_name));
}

void Geometory::SetPixel(const char* _name)
{
	pPS = static_cast<PixelShader*>(ShaderPool::GetShader(_name));
}
