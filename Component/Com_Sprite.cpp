#include "Com_Sprite.h"
#include "../DirectX/renderer.h"
#include "../GameObject/GameObject.h"
#include "../System/TextureCreate.h"
#include "../Utility.h"

using namespace DirectX::SimpleMath;

Com_Sprite::Com_Sprite()
{
	p_mSRV = new ShaderResourceView();
	bShadowEnable = false;
}

void Com_Sprite::Init()
{
	VERTEX_SPRITE vertex[SpriteVertexNum];
	Color Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Vector2 vx[SpriteVertexNum];
	float Half = 0.5f;

	Vector3 center = p_mObject->GetPosition();

	// トランスフォームコンポーネントに基づいてポジションとスケールを設定
	vx[0] = Vector2(-mSize.x * Half, -mSize.y * Half);
	vertex[0].Diffuse = Diffuse;
	vertex[0].TexCoord = Vector2(fCurrentx, fCurrenty);

	vx[1] = Vector2(mSize.x * Half, -mSize.y * Half);
	vertex[1].Diffuse = Diffuse;
	vertex[1].TexCoord = Vector2(fCurrentz, fCurrenty);

	vx[2] = Vector2(-mSize.x * Half, mSize.y * Half);
	vertex[2].Diffuse = Diffuse;
	vertex[2].TexCoord = Vector2(fCurrentx, fCurrentw);

	vx[3] = Vector2(mSize.x * Half, mSize.y * Half);
	vertex[3].Diffuse = Diffuse;
	vertex[3].TexCoord = Vector2(fCurrentz, fCurrentw);

	// 回転計算
	Matrix mat = Matrix::CreateFromYawPitchRoll(p_mObject->p_mTransform->mRotation.y, p_mObject->p_mTransform->mRotation.x, p_mObject->p_mTransform->mRotation.z);

	for (int i = 0; i < SpriteVertexNum; i++)
	{
		vx[i] = Vector2::Transform(vx[i], mat);
		vx[i] = vx[i] + center;
		vertex[i].Position = vx[i];
	}

	HRESULT hr = CreateVertexBufferWrite(Renderer::GetDevice(), sizeof(VERTEX_SPRITE), SpriteVertexNum, vertex, &p_mVertexBuffer);
	if (FAILED(hr))
	{
		NEWDEBUGLOG("Spriteの頂点バッファ作成に失敗" << p_mObject->sObjectName);
	}

}


void Com_Sprite::Uninit()
{
	SAFE_RELEASE(p_mVertexBuffer);
	if (bRelease)
	{
		p_mSRV->Uninit();
	}
	delete p_mSRV;
}

void Com_Sprite::Update()
{
	if (bUpdate)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		VERTEX_SPRITE* vx = nullptr;
		HRESULT hr = Renderer::GetDeviceContext()->Map(
			p_mVertexBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr))
		{
			vx = (VERTEX_SPRITE*)msr.pData;

			// 回転計算
			Vector3 Pos = p_mObject->GetPosition();
			Vector3 Rot = p_mObject->GetRotation();
			Matrix mat = Matrix::CreateFromYawPitchRoll(Rot.y, Rot.x, Rot.z);
			float Half = 0.5f;

			Vector2 Center = Vector2(p_mObject->GetPosition().x, p_mObject->GetPosition().y);

			vx->Position = Vector2(-mSize.x * Half, -mSize.y * Half);
			vx->Diffuse = mColor;
			vx->TexCoord = Vector2(fCurrentx, fCurrenty);
			vx->Position = Vector2::Transform(vx->Position, mat);
			vx->Position += Center;
			vx++;
			vx->Position = Vector2(mSize.x * Half, -mSize.y * Half);
			vx->Diffuse = mColor;
			vx->TexCoord = Vector2(fCurrentz, fCurrenty);
			vx->Position = Vector2::Transform(vx->Position, mat);
			vx->Position += Center;
			vx++;
			vx->Position = Vector2(-mSize.x * Half, mSize.y * Half);
			vx->Diffuse = mColor;
			vx->TexCoord = Vector2(fCurrentx, fCurrentw);
			vx->Position = Vector2::Transform(vx->Position, mat);
			vx->Position += Center;
			vx++;
			vx->Position = Vector2(mSize.x * Half, mSize.y * Half);
			vx->Diffuse = mColor;
			vx->TexCoord = Vector2(fCurrentz, fCurrentw);
			vx->Position = Vector2::Transform(vx->Position, mat);
			vx->Position += Center;

			Renderer::GetDeviceContext()->Unmap(p_mVertexBuffer, 0);
		}
	}
}

void Com_Sprite::Draw()
{
	Bind();

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_SPRITE);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &p_mVertexBuffer, &stride, &offset);

	// テクスチャ設定
	ID3D11ShaderResourceView* srv = p_mSRV->GetSRV();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &srv);

	// プリミティブトポロジ設定
	Renderer::SetTopologyTriangleStrip();

	// マテリアル設定
	Renderer::SetMaterial(mMaterial);

	Renderer::GetDeviceContext()->Draw(SpriteVertexNum, 0);

}

void Com_Sprite::SetSeparateNum(int _u, int _v)
{
	if (_u != 0)
	{
		iSeparateX = _u;
	}
	if (_v != 0)
	{
		iSeparateY = _v;
	}
}

void Com_Sprite::SetCurrent(int _val)
{
	iCurrent = _val;

	//テクスチャ座標算出

	// UV分割数を基に表示位置の計算
	int SeparateNum = iSeparateX * iSeparateY;
	if (iCurrent > SeparateNum)
	{
		iCurrent = iCurrent % SeparateNum;
	}

	int x = iCurrent % iSeparateX;
	float SepX;
	SepX = 1.0f / iSeparateX;
	fCurrentx = SepX * (x - 1);
	fCurrentz = SepX * x;

	float SepY;
	SepY = 1.0f / iSeparateY;
	int y = 1;
	int num = iCurrent;
	while (true)
	{
		if (num > iSeparateX)
		{
			y++;
			num = num - iSeparateX;
		}
		else
		{
			break;
		}
	}
	fCurrenty = SepY * (y - 1);
	fCurrentw = SepY * y;
}


void Com_Sprite::SetTexture(const char* _name)
{
	// テクスチャを設定
	p_mSRV->Create(_name);

	mMaterial.TextureEnable = true;
}

void Com_Sprite::SetTexture(ShaderResourceView* _psrv)
{
	p_mSRV->SetSRV(_psrv->GetSRV());
}

void Com_Sprite::SetSize(float _x, float _y)
{
	mSize.x = _x;
	mSize.y = _y;
}

void Com_Sprite::SetVertexColor(float _r, float _g, float _b, float _a)
{
	mColor.x = _r;
	mColor.y = _g;
	mColor.z = _b;
	mColor.w = _a;
}

void Com_Sprite::SetMaterial(MATERIAL mat)
{
	// マテリアルを設定
	mMaterial = mat;
}

void Com_Sprite::SetAllbedoColor(DirectX::SimpleMath::Color _color)
{
	mMaterial.AlbedoColor = _color;
}

void Com_Sprite::SetEmission(DirectX::SimpleMath::Color _color)
{
	mMaterial.Emission = _color;
}

void Com_Sprite::Create()
{
	
}

ID3D11ShaderResourceView* Com_Sprite::GetSRV()
{
	return p_mSRV->GetSRV();
}
