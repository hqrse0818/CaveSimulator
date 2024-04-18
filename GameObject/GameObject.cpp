#include "GameObject.h"
#include "../DirectX/renderer.h"
#include "../System/fpscontrol.h"
#include "../ComponentHeader.h"

using namespace std;

Wint GameObject::s_mAllNum = { 1, 1 };
Scene* GameObject::s_p_mScene = nullptr;

// コンストラクタ
GameObject::GameObject(string _name)
	: GameObject()
{
	sObjectName = _name;
}

GameObject::GameObject(const char* _name)
	:  GameObject()
{
	sObjectName = _name;
}

GameObject::GameObject()
{
	mID = s_mAllNum;

	s_mAllNum.second++;
	if (s_mAllNum.second > ObjectIDSecondMax)
	{
		s_mAllNum.first++;
		s_mAllNum.second = 1;
	}

	// transformの実態を作る
	p_mTransform = new Com_Transform();

	p_mTransform->p_mObject = this;

	// コンポーネントに追加する
	p_mComponentList.push_back(p_mTransform);

	p_mParent = nullptr;
}

Component* GameObject::GetComponent(const type_info& type)
{
	for (auto a : p_mComponentList)
	{
		// 型が同じならreturn
		if (typeid(*a) == type)
		{
			return a;
		}
	}
	
	// なかった場合nullを返す
	return nullptr;
}

void GameObject::AddComponent(Component* pCom)
{
	p_mComponentList.push_back(pCom);
	pCom->p_mObject = this;

	if (typeid(*pCom) == typeid(Com_BoxCollider))
	{
		mColType = ColliderForm::Box;
	}
	else if (typeid(*pCom) == typeid(Com_BoxCollider2D))
	{
		mColType = ColliderForm::Box2D;
	}
	else if (typeid(*pCom) == typeid(Com_SphereCollider))
	{
		mColType = ColliderForm::Sphere;
	}
	else if (typeid(*pCom) == typeid(Com_CircleCollider))
	{
		mColType = ColliderForm::Circle;
	}
	else if (typeid(*pCom) == typeid(Com_CylinderCollider))
	{
		mColType = ColliderForm::Cylinder;
	}
}

void GameObject::InitBase()
{
	// コンポーネントの初期化処理
	for (auto com : p_mComponentList)
	{
		if (com->bEnable)
		{
			com->Init();
		}
	}
	for (auto Obj : p_mChildren)
	{
		if (Obj->GetActive())
		{
			Obj->InitBase();
		}
	}

	// オブジェクト初期化処理
	Init();
}

void GameObject::StartBase()
{
	// コンポーネントの開始処理
	for (auto com : p_mComponentList)
	{
		if (com->bEnable)
		{
			com->Start();
		}
	}
	for (auto Obj : p_mChildren)
	{
		if (Obj->GetActive())
		{
			Obj->StartBase();
		}
	}

	// オブジェクトの開始処理
	Start();
}

void GameObject::UpdateBase()
{
	// コンポーネントの更新処理
	if (bActive)
	{
		for (auto com : p_mComponentList)
		{
			if (com->bEnable)
			{
				com->Update();
			}
		}
		for (auto Obj : p_mChildren)
		{
			Obj->UpdateBase();
		}
	}

	// オブジェクト更新処理
	Update();

	mLastCollisionvector.clear();
	mLastCollisionvector = mCollisionvector;
	mCollisionvector.clear();
}

void GameObject::Uninit()
{
	// 自身のコンポーネントを開放
	for (auto com : p_mComponentList)
	{
			com->Uninit();
			SAFE_NULL(com);
	}
	// 子オブジェクトのコンポーネントを開放
	for (auto Obj : p_mChildren)
	{
		Obj->Uninit();
		Obj->p_mComponentList.clear();

		Obj->p_mParent = nullptr;

		SAFE_NULL(Obj);
	}

	p_mComponentList.clear();
}

void GameObject::PreDrawBase()
{
	if (bActive)
	{
		for (auto com : p_mComponentList)
		{
			if (com->bEnable)
			{
				com->PreDraw();
			}
		}
		for (auto Obj : p_mChildren)
		{
			Obj->PreDrawBase();
		}
	}

	// オブジェクト描画前処理
	PreDraw();
}

void GameObject::Draw()
{
	for (auto com : p_mComponentList)
	{
		if (com->bEnable)
		{
			com->Draw();
		}
	}
}

void GameObject::ShadowDraw()
{
	for (auto com : p_mComponentList)
	{
		if (com->bEnable)
		{
			com->ShadowDraw();
		}
	}
}

void GameObject::OnCollisionEnter(GameObject* _obj)
{
	if (bActive)
	{
		for (auto& com : p_mComponentList)
		{
			if(com->bEnable)
			com->OnCollisionEnter(_obj);
		}
		for (auto obj : p_mChildren)
		{
			obj->OnCollisionEnter(_obj);
		}
	}
}

void GameObject::OnCollisionStay(GameObject* _obj)
{
	if (bActive)
	{
		for (auto& com : p_mComponentList)
		{
			if(com->bEnable)
			com->OnCollisionStay(_obj);
		}
		for (auto obj : p_mChildren)
		{
			obj->OnCollisionStay(_obj);
		}
	}
}

void GameObject::OnCollisionExit(GameObject* _obj)
{
	if (bActive)
	{
		for (auto& com : p_mComponentList)
		{
			if(com->bEnable)
			com->OnCollisionExit(_obj);
		}
		for (auto obj : p_mChildren)
		{
			obj->OnCollisionExit(_obj);
		}
	}
}

void GameObject::DrawBase(DirectX::SimpleMath::Matrix ParentMatrix)
{
	if (bActive)
	{
		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(p_mTransform->mScale);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(p_mTransform->mRotation.y, p_mTransform->mRotation.x, p_mTransform->mRotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(p_mTransform->mPosition);
		world = scale * rot * trans * ParentMatrix;

		for (auto child : p_mChildren)
		{
			child->DrawBase(world);
		}

		Renderer::SetWorldMatrix(&world);

		Draw();
	}
}

void GameObject::ShadowDrawBase(DirectX::SimpleMath::Matrix _Matrix)
{
	if (bActive)
	{
		// マトリクス設定
		DirectX::SimpleMath::Matrix world, scale, rot, trans;
		scale = DirectX::SimpleMath::Matrix::CreateScale(p_mTransform->mScale);
		rot = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(p_mTransform->mRotation.y, p_mTransform->mRotation.x, p_mTransform->mRotation.z);
		trans = DirectX::SimpleMath::Matrix::CreateTranslation(p_mTransform->mPosition);
		world = scale * rot * trans * _Matrix;

		for (auto child : p_mChildren)
		{
			child->ShadowDrawBase(world);
		}

		Renderer::SetWorldMatrix(&world);

		ShadowDraw();
	}
}


void GameObject::SetPosition(float _x, float _y, float _z)
{
	p_mTransform->SetPosition(_x, _y, _z);
}

void GameObject::SetPosition(DirectX::SimpleMath::Vector3 _val)
{
	p_mTransform->mPosition = _val;
}

void GameObject::SetRotation(float _x, float _y, float _z)
{
	p_mTransform->SetRotation(_x, _y, _z);
}

void GameObject::SetRotation(DirectX::SimpleMath::Vector3 _val)
{
	p_mTransform->mRotation = _val;
}

void GameObject::SetScale(float _x, float _y, float _z)
{
	p_mTransform->SetScale(_x, _y, _z);
}

void GameObject::SetScale(DirectX::SimpleMath::Vector3 _val)
{
	p_mTransform->mScale = _val;
}

DirectX::SimpleMath::Vector3 GameObject::GetPosition()
{
	return p_mTransform->mPosition;
}

DirectX::SimpleMath::Vector3 GameObject::GetRotation()
{
	return p_mTransform->mRotation;
}

DirectX::SimpleMath::Vector3 GameObject::GetScale()
{
	return p_mTransform->mScale;
}
