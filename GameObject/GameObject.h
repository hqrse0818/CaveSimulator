#pragma once

#include <list>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../Component//Component.h"
#include "../Component/Com_Transform.h"

typedef std::list<Component*> Componentlist;
typedef std::list<GameObject*> Objectlist;
typedef std::vector<GameObject*> Objectvector;
typedef std::unordered_map<std::string, GameObject*> Objectmap;

class Scene;


enum class ColliderForm
{
	None, Box, Sphere, Cylinder,Box2D, Circle
};

// �I�u�W�F�N�gID��second�̍ő吔
#define ObjectIDSecondMax 1000

struct Wint
{
	unsigned int first;
	unsigned int second;
};

class GameObject
{
protected:
	// �Q�[���I�u�W�F�N�g�����ۂɎ���protected�ϐ�
	// ������
	static Wint s_mAllNum;

	// �I�u�W�F�N�gID
	Wint mID;

	// �A�N�e�B�x�[�V����
	bool bActive = true;

	static Scene* s_p_mScene;

	// �R���|�[�l���g���X�g
	Componentlist p_mComponentList;

public:
	// �q�I�u�W�F�N�g���X�g
	Objectlist p_mChildren;

	// �j��\��
	bool bDestroy = false;
	ColliderForm mColType = ColliderForm::None;

	// �I�u�W�F�N�g�̃q�b�g���X�g
	Objectvector mCollisionvector;
	Objectvector mLastCollisionvector;

	// �I�u�W�F�N�g��(�f�o�b�O�p)
	std::string sObjectName;
	// �I�u�W�F�N�g�̃^�O
	std::string sTag;

	std::string sCollisionTag;

	// �e�I�u�W�F�N�g
	GameObject* p_mParent;

	// Com_Transform�̊ȈՃA�N�Z�X
	Com_Transform* p_mTransform;

public:
	// �R���X�g���N�^
	GameObject(const char* _name);
	GameObject(std::string _name);
	GameObject();
	// �f�X�g���N�^
	virtual ~GameObject() {};

	// �Q�[���I�u�W�F�N�g�����ۂɎ���public�֐�
	
	// �x�[�X����������
	void InitBase();
	// ����������
	virtual void Init() {};
	// �x�[�X�J�n����
	void StartBase();
	// �J�n����
	virtual void Start() {};
	// �x�[�X�X�V����
	void UpdateBase();
	// �X�V����
	virtual void Update() {};
	// �x�[�X�`��O����
	void PreDrawBase();
	// �`��O�X�V����
	virtual void PreDraw() {};
	// �x�[�X�����_�����O����
	void DrawBase(DirectX::SimpleMath::Matrix ParentMatrix);
	// �`�揈��
	virtual void Draw();


	// �[�x�l�`��
	virtual void ShadowDraw();

	// �Փˎ�����
	virtual void OnCollisionEnter(GameObject* _obj);
	// �Փ˒�����
	virtual void OnCollisionStay(GameObject* _obj);
	// �Փˌ㏈��
	virtual void OnCollisionExit(GameObject* _obj);

	// �������
	virtual void Uninit();

	// �A�N�e�B�u�̃Z�b�^�[
	void SetActive(bool _val){bActive = _val;}
	// �A�N�e�B�u�̃Q�b�^�[
	bool GetActive(){return bActive;}

	// �R���|�[�l���g�p�̃N���X�e���v���[�g
	template<typename T>
	T* GetComponent()
	{
		for (auto a : p_mComponentList)
		{
			// �^�������Ȃ�return
			if (typeid(*a) == typeid(T))
			{
				return static_cast<T*>(a);
			}
		}
		return nullptr;
	};

	// �R���|�[�l���g�̎擾
	Component* GetComponent(const type_info& type);

	// �R���|�[�l���g�̒ǉ�
	void AddComponent(Component* pCom);

	void AddChild(GameObject* _obj)
	{
		p_mChildren.push_back(_obj);
		_obj->p_mParent = this;
	};

	bool Destroy()
	{
		if (bDestroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	Wint GetID()
	{
		return mID;
	}

	
	void ShadowDrawBase(DirectX::SimpleMath::Matrix _Matrix);

	static void SetScene(Scene* _scene)
	{
		s_p_mScene = _scene;
	}
	static Scene* GetScene()
	{
		return s_p_mScene;
	}

	// �g�����X�t�H�[���ȈՃA�N�Z�X
	void SetPosition(float _x, float _y, float _z = 0);
	void SetPosition(DirectX::SimpleMath::Vector3 _val);
	void SetRotation(float _x, float _y, float _z);
	void SetRotation(DirectX::SimpleMath::Vector3 _val);
	void SetScale(float _x, float _y, float _z);
	void SetScale(DirectX::SimpleMath::Vector3 _val);

	DirectX::SimpleMath::Vector3 GetPosition();
	DirectX::SimpleMath::Vector3 GetRotation();
	DirectX::SimpleMath::Vector3 GetScale();
};
