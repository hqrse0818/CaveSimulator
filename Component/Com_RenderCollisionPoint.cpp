#include "Com_RenderCollisionPoint.h"
#include "../GameObject/GameObject.h"
#include "Com_PointCollider.h"

using namespace DirectX::SimpleMath;

void Com_RenderCollisionPoint::Init()
{
	Com_PointCollider* col = p_mObject->GetComponent<Com_PointCollider>();
	Point3D& point = *col->GetCollider();

	mRenderVertex = Vector3(point.Center);

	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vector3);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = &mRenderVertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mVertexBuffer);
	}
}

void Com_RenderCollisionPoint::Update()
{
	if (p_mObject->GetComponent<Com_PointCollider>()->GetHit())
	{
		SetPixelShaderHit();
	}
	else
	{
		SetPixelShaderNoHit();
	}
}

void Com_RenderCollisionPoint::Draw()
{
	Com_RenderCollider::Draw();

	Renderer::GetDeviceContext()->Draw(1, 0);
}