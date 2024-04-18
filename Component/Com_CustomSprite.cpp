#include "Com_CustomSprite.h"
#include "../DirectX/renderer.h"
#include "../GameObject/GameObject.h"
#include "../System/TextureCreate.h"

using namespace DirectX::SimpleMath;

void Com_CustomSprite::Init()
{
	Create();
}

void Com_CustomSprite::Draw()
{
	Com_Sprite::Draw();
}

void Com_CustomSprite::Create()
{
	VERTEX_SPRITE vertex[SpriteVertexNum];

	Vector3 StartPoint;
	StartPoint = p_mObject->p_mTransform->mPosition;
	float Half = 0.5f;

	switch (mType)
	{
	case Com_CustomSprite::CustomType::Center:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y - mSize.y * Half);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y - mSize.y * Half);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y + mSize.y * Half);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y + mSize.y * Half);

		break;
	case Com_CustomSprite::CustomType::CenterTop:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y + mSize.y);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y + mSize.y);
		break;
	case Com_CustomSprite::CustomType::CenetrBottom:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y - mSize.y);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y - mSize.y);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x * Half, StartPoint.y);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x * Half, StartPoint.y);
		break;
	case Com_CustomSprite::CustomType::Left:
		vertex[0].Position = Vector2(StartPoint.x, StartPoint.y - mSize.y * Half);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y - mSize.y * Half);
		vertex[2].Position = Vector2(StartPoint.x, StartPoint.y + mSize.y * Half);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y + mSize.y * Half);
		break;
	case Com_CustomSprite::CustomType::LeftTop:
		vertex[0].Position = Vector2(StartPoint.x, StartPoint.y);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y);
		vertex[2].Position = Vector2(StartPoint.x, StartPoint.y + mSize.y);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y + mSize.y);
		break;
	case Com_CustomSprite::CustomType::LeftBottom:
		vertex[0].Position = Vector2(StartPoint.x, StartPoint.y - mSize.y);
		vertex[1].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y - mSize.y);
		vertex[2].Position = Vector2(StartPoint.x, StartPoint.y);
		vertex[3].Position = Vector2(StartPoint.x + mSize.x, StartPoint.y);
		break;
	case Com_CustomSprite::CustomType::Right:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y - mSize.y * Half);
		vertex[1].Position = Vector2(StartPoint.x, StartPoint.y - mSize.y * Half);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y + mSize.y * Half);
		vertex[3].Position = Vector2(StartPoint.x, StartPoint.y + mSize.y * Half);
		break;
	case Com_CustomSprite::CustomType::RightTop:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y);
		vertex[1].Position = Vector2(StartPoint.x, StartPoint.y);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y + mSize.y);
		vertex[3].Position = Vector2(StartPoint.x, StartPoint.y + mSize.y);
		break;
	case Com_CustomSprite::CustomType::RightBottom:
		vertex[0].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y - mSize.y);
		vertex[1].Position = Vector2(StartPoint.x, StartPoint.y - mSize.y);
		vertex[2].Position = Vector2(StartPoint.x - mSize.x, StartPoint.y);
		vertex[3].Position = Vector2(StartPoint.x, StartPoint.y);
		break;
	}
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	vertex[0].TexCoord = Vector2(fCurrentx, fCurrenty);
	vertex[1].TexCoord = Vector2(fCurrentz, fCurrenty);
	vertex[2].TexCoord = Vector2(fCurrentx, fCurrentw);
	vertex[3].TexCoord = Vector2(fCurrentz, fCurrentw);

	// 回転計算
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position = vertex[i].Position - StartPoint;
	}

	Matrix mat = Matrix::CreateFromYawPitchRoll(p_mObject->p_mTransform->mRotation.y, p_mObject->p_mTransform->mRotation.x, p_mObject->p_mTransform->mRotation.z);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position = Vector2::Transform(vertex[i].Position, mat);
		vertex[i].Position = vertex[i].Position + StartPoint;
	}

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_SPRITE) * SpriteVertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &p_mVertexBuffer);
}
