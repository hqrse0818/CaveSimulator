#include "OBJ_Transition.h"
#include "../ComponentHeader.h"
#include "../System/TextureCreate.h"
#include "../main.h"
#include "../Format.h"
#include "../System/Time.h"
#include "../TextureFilePath.h"

using namespace DirectX::SimpleMath;

OBJ_Transition::OBJ_Transition()
{
	Com_Shader* Shader_buf = new Com_Shader();
	Shader_buf->p_mVS->Load("asset\\unlitTextureVS.cso");
	Shader_buf->p_mPS->Load("asset\\unlitTexturePS.cso");
	AddComponent(Shader_buf);

	Sprite_buf = new Com_Sprite();
	Sprite_buf->SetTexture(Tex_WhiteBase);
	MATERIAL mat{};
	mat.AlbedoColor = mColor;
	Sprite_buf->SetMaterial(mat);
	AddComponent(Sprite_buf);

	p_mTransform->SetScale(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f);
	p_mTransform->mPosition.x = SCREEN_WIDTH / 2;
	p_mTransform->mPosition.y = SCREEN_HEIGHT / 2;
}

OBJ_Transition::OBJ_Transition(const char* _name)
	: OBJ_Transition()
{
	sObjectName = _name;
}

void OBJ_Transition::Update()
{
	switch (mState)
	{
	case OBJ_Transition::FadeState::Stop:
		break;
	case OBJ_Transition::FadeState::In:
		mColor.w -= Time->GetDeltaTime() / FadeTime;
		if (mColor.w <= 0.0f)
		{
			mState = FadeState::Stop;
		}
		break;
	case OBJ_Transition::FadeState::Out:
		mColor.w += Time->GetDeltaTime() / FadeTime;
		if (mColor.w >= 1.0f)
		{
			mState = FadeState::Finish;
		}
		break;
	case OBJ_Transition::FadeState::Finish:
		break;
	default:
		break;
	}

	MATERIAL mat{};
	mat.AlbedoColor = mColor;
	Sprite_buf->SetMaterial(mat);

	GameObject::UpdateBase();
}
