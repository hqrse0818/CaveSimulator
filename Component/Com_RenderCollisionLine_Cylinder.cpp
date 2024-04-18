#include "Com_RenderCollisionLine_Cylinder.h"
#include "Com_CylinderCollider.h"
#include "../GameObject/GameObject.h"

using namespace DirectX::SimpleMath;

void Com_RenderCollisionLine_Cylinder::Init()
{
	Com_CylinderCollider* cyCol = p_mObject->GetComponent<Com_CylinderCollider>();
	//Cylinder tmp = cyCol->GetCylinder();

	//fRadius = tmp.Radius;
	//height = tmp.Height;
	//Center = tmp.Center;

	float angle = Euler_To_Radian(360.0f / 8 * 0);
	mRenderVertex[0].x = fRadius * sinf(angle);
	mRenderVertex[0].z = fRadius * cosf(angle);
	mRenderVertex[0].y = fHeight / 2;

	mRenderVertex[15] = mRenderVertex[0];

	// 8角形としてそれぞれの頂点位置を求める
	for (int i = 1; i < 8; i++)
	{
		angle = (float)Euler_To_Radian(360.0f / 8 * i);

		mRenderVertex[i * 2].x = fRadius * sinf(angle);
		mRenderVertex[i * 2].z = fRadius * cosf(angle);
		mRenderVertex[i * 2].y = fHeight / 2;

		mRenderVertex[i * 2 - 1] = mRenderVertex[i * 2];
	}
	// y座標以外は上面と一緒
	for (int i = 16; i < 32; i++)
	{
		mRenderVertex[i] = mRenderVertex[i - 16];
		mRenderVertex[i].y = -fHeight / 2;
	}

	// 縦につなぐ
	for (int i = 32, s = 0; i < 48; i++)
	{
		if (i % 2 == 0)
		{
			mRenderVertex[i] = mRenderVertex[s * 2];
		}
		else
		{
			mRenderVertex[i] = mRenderVertex[s * 2 + 16];
			s++;
		}
	}

	for (int i = 0; i < 48; i++)
	{
		//RenderVertex[i].y += Center.y;
	}



	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vector3) * 48;	// 頂点データの数分だけ指定
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = mRenderVertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mVertexBuffer);
	}
}

void Com_RenderCollisionLine_Cylinder::Update()
{
	if (p_mObject->GetComponent<Com_CylinderCollider>()->GetHit())
	{
		SetPixelShaderHit();
	}
	else
	{
		SetPixelShaderNoHit();
	}
}

void Com_RenderCollisionLine_Cylinder::Draw()
{
	Com_RenderCollider::Draw();

	// 描画
	Renderer::GetDeviceContext()->Draw(48, 0);
}
