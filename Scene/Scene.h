#pragma once
#include "../GameObject/GameObject.h"
#include "../ComponentHeader.h"
#include "../System/ObjectManager.h"
#include "../System/Input.h"
#include "../System/TextureCreate.h"
#include <unordered_map>
#include <array>
#include <string>
#include <thread>
#include <iostream>
#include "../Debug.h"
#include "../GameObject/DynamicEnvironmentMap.h"
#include "../DirectX/CubeMapGenerator.h"

//#define ObjectLayerNum 8
// ���C���[�o�^�T�v
// �Q�[���x�[�X��
// 0 �J�����ȂǗD�揇�ʍō�
// 1 �v���C���[��G�Ȃǂ�Movable�œ����蔻���������
// 2 �Œ�I�u�W�F�N�g�œ����蔻���������
// 3 ���̑��I�u�W�F�N�g
// 5 UI�X�v���C�g�����蔻��̂Ȃ�����
// 6  UI�X�v���C�gMovable���蔻��̂������
// 7 UI�X�v���C�g�Œ蓖���蔻��̂������
// 8 UI�X�v���C�g���̑�


#define RenderingLayerNum 6
// �f�B�t�@�[�h�����_�����O�x�[�X��
// �`��菇�Ő���(�R���|�[�l���g�x�[�X)
// 0 �J�����̕`��(WVP�ϊ��s��̒萔�o�b�t�@�X�V)
#define ObjListIndex_Camera 0
// 1 �ÓI�I�u�W�F�N�g
#define ObjListIndex_Static 1
// 2 ���I�I�u�W�F�N�g
#define ObjListIndex_Skeletal 2
// 3 �������I�u�W�F�N�g
#define ObjListIndex_Transparent 3
// 4 �G�t�F�N�g
#define ObjListIndex_Effect 4
// 5 UI
#define ObjListIndex_UI 5

// ��������`��菇
// GBuffer�p�X�̎��s(1, 2�I�u�W�F�N�g)
// ��
// �f�B�t�@�[�h�����_�����O�p�����_�[�^�[�Q�b�g�ɐ؂�ւ�
// ��
// �f�B�t�@�[�h�̃p�X���s
// ��
// �������I�u�W�F�N�g��G�t�F�N�g�̕`��(�t�H���[�h)
// ��
// �|�X�g�v���Z�X
// ��
// UI�̕`��

class Scene
{
protected:// �ϐ�

	// �z�u�I�u�W�F�N�g�⃉�C�g�̃f�[�^�̓ǂݍ��݂Ŏg�p���閼�O
	std::string mSceneName = "NoName";

	// �`��^�C�v(�|�X�g�G�t�F�N�g�Ȃǂő��₷�ꍇ�͂����𑝂₷)
	enum class SceneDrawType
	{
		GBuffer,
		Deffered,
		DefferedPoint,
		NoTexture,
		OnlyEnvironment,
		CubeMap,
		EnvFront,
		EnvBack,
		EnvRight,
		EnvLeft,
		EnvTop,
		EnvBottom,
		Max
	};

	enum class GBufferType
	{
		Main,
		EnvFront,
		EnvBack,
		EnvRight,
		EnvLeft,
		EnvTop,
		EnvBottom,
		Max
	};

	enum class EnvironmentCurrentRenderType
	{
		Front,
		Back,
		Right,
		Left,
		Top,
		Bottom,
		Max
	};

	// ���}�b�s���O�̃��x��
	enum class SceneEnvironmentMappingType
	{
		None,
		UntilOpacity,
		UntilToransparent,
		UntilEffect,
		Max
	};

	bool bFullScreen = false;
	bool bFullScreenOld = false;

	//	�I�u�W�F�N�g���X�g
	std::array<Objectlist, RenderingLayerNum> mSceneObjectList;
	// �����_�����O�x�[�X
	//std::array<Objectlist,LayerNum>

	// ���X�g�̃T�C�Y
	int ListSize = 0;
	// ���̃V�[���̃��[�h
	bool bLoadNextScene = false;

	// ��v�I�u�W�F�N�g���X�g(unorderd_map�ŊǗ��擾������)
	ObjectManager mKeyObjectList;

	// �V�[���œ����蔻����s����
	bool bCheckCol = false;

	GameObject* pEmpty{};
	Com_SimpleTranslate* pTransCom{};

	// �x�[�X�V�[���ɂ�����J�����ƃ^�[�Q�b�g�I�u�W�F�N�g�̑���
	bool bOperateCamera = true;

	GameObject* pCamera{};
	Com_Camera* pMainCamera{};
	float fAngleSpeed = 1.0f;

	// �`��^�C�v
	SceneDrawType mDrawType = SceneDrawType::Deffered;
	GBufferType mBufferType = GBufferType::Main;
	EnvironmentCurrentRenderType mEnvCurrent = EnvironmentCurrentRenderType::Front;

	//GUI�̕\��
	bool bShowGUI = false;
	// �V�[���f�[�^�̕\��
	bool bShowSceneData = false;
	// �V�[���̃I�u�W�F�N�g�f�[�^�̕\��
	bool bShowObjectData = true;
	// ���f�����X�g�̕\��
	bool bShowModelData = false;
	// �}�e���A���f�[�^�̕\��
	bool bShowMaterialData = false;
	// ���C�g�̃��X�g�̕\��
	bool bShowLightData = false;

	// GBuffer�p�I�u�W�F�N�g
	GameObject* pNormal{};
	GameObject* pAlbedo{};
	GameObject* pWorld{};
	GameObject* pMaterial{};
	GameObject* pDeffered{};
	GameObject* pResultDef{};
	// GBuffer�p�X�v���C�g
	Com_Sprite* pNormalSprite{};
	Com_Sprite* pAlbedoSprite{};
	Com_Sprite* pWorldSprite{};
	Com_Sprite* pMaterialSprite{};
	Com_Sprite* pDefferedSprite{};
	Com_Sprite* pResultDefSprite{};

	// GBuffer
	RenderTarget* pGBufferNormal{};
	RenderTarget* pGBufferAlbedo{};
	RenderTarget* pGBufferWorld{};
	RenderTarget* pGBufferMaterial{};

	// �L���[�u�}�b�v�W�F�l���[�^�[
	CubeMapGenerator* pCubeMapGenerator{};

	// ���}�b�s���O�p�I�u�W�F�N�g
	DynamicEnvironmentMap* p_mFrontMap{};
	DynamicEnvironmentMap* p_mBackMap{};
	DynamicEnvironmentMap* p_mRightMap{};
	DynamicEnvironmentMap* p_mLeftMap{};
	DynamicEnvironmentMap* p_mTopMap{};
	DynamicEnvironmentMap* p_mBottomMap{};
	// �L���[�u�}�b�v�p�����_�[�^�[�Q�b�g
	RenderTarget* pRTVCubeMap{};

	// �L���[�u�}�b�v�p�I�u�W�F�N�g
	GameObject* pCubeMapObj{};
	Com_Sprite* pCubeMapSprite{};

	// �e�X�g�X�v���C�g
	GameObject* p_mTestObject{};
	Com_Sprite* p_mTestSprite{};

	// �@���}�b�v�p�X�v���C�g
	Com_Sprite* p_mCrystalNormalSprite{};

protected:// �֐�
	void SetMainCamera(Com_Camera* _cam)
	{
		pMainCamera = _cam;
	}

	// �����蔻�菈���̎��s
	virtual void CollisionUpdate();

	// GUI
	void GuiUpdateBase();

	// �V�[���f�[�^GUI
	void ShowGuiSceneData();
	// �I�u�W�F�N�g�f�[�^GUI
	void ShowGuiObjectData();
	// �z�u�����I�u�W�F�N�g�f�[�^�̃t�@�C���ۑ�
	void SaveLevelData();
	// �I�u�W�F�N�g�f�[�^�̓ǂݍ���
	void ReadLevelData();
	// ���C�g�̃f�[�^�̓ǂݍ���
	void ReadLightData();
	// �}�e���A���f�[�^�̓ǂݍ���
	void ReadMaterialData(const char* _FileName);
	// ���}�b�s���O�����֐�
	void CreateEnvironmentMap();
	// ���}�b�v�Ƀ����_�����O����I�u�W�F�N�g�̕`��
	void RenderEnvironmentObject(DirectX::SimpleMath::Matrix _Matrix);
	// �L���[�u�}�b�v�̍쐬
	void CreateCubeMap();

	std::string CreateLevelFileName();
	std::string CreateLightFileName();

public:// �֐�

	Com_Camera* GetMainCamera()
	{
		return pMainCamera;
	}

	Scene()
	{
		for (auto& List : mSceneObjectList)
		{
			List.clear();
		}
	};
	virtual ~Scene() {};
	// �V�F�[�_�[�̓ǂݍ��݂Ȃ�
	virtual void PreInit() {};
	// �I�u�W�F�N�g�o�^�A�ϐ��������Ȃǂ̏���������
	virtual void Init() {};
	// ���[�v�J�n�O����(BGM�I���Ȃ�)
	virtual void Start() {};
	// �X�V����
	virtual void Update() {};
	// �`��O�ɍs������
	virtual void PreDraw() {};
	// 3D�I�u�W�F�N�g�`��㏈��
	virtual void Draw3D() {};
	// 2D�I�u�W�F�N�g�`��㏈��
	virtual void Draw2D() {};
	// Gui�̕`��
	virtual void GUIDraw() {};

	virtual void MakeShadowMap();
	virtual void Uninit() {};

	// �������̃x�[�X����
	void InitBase();
	// �J�n�̃x�[�X����
	void StartBase()
	{
		// �Q�[���I�u�W�F�N�g�̃^�[�Q�b�g�V�[���̐ݒ�
		GameObject::SetScene(this);

		// �I�u�W�F�N�g���X�g�̐�������
		for (auto& List : mSceneObjectList)
		{
			// ���X�g�̒��g�̐����������
			for (auto& obj : List)
			{
				obj->StartBase();
			}
		}

		Start();
	}
	// �J���̃x�[�X����
	void UninitBase();
	// �X�V�̃x�[�X����
	void UpdateBase();
	// �`��O�����̃x�[�X����
	void PreDrawBase()
	{
		for (auto& List : mSceneObjectList)
		{
			for (auto& obj : List)
			{
				obj->PreDrawBase();
			}
		}

		PreDraw();
	}
	// �`��̃x�[�X����
	void DrawBase();

	// �ʏ�̃I�u�W�F�N�g�o�^
	void AddGameObject(GameObject* _obj, int Layer)
	{
		mSceneObjectList[Layer].emplace_back(_obj);
	}
	// �i�[�z��̌���
	int LayerCheck(GameObject* _obj);
	// �L�[�I�u�W�F�N�g�Ƃ��ēo�^
	// unordered_map�ɓo�^(�擾�܂ł̏����͑�����for���ɂ͓K���Ă��Ȃ�)
	
	void AddKeyObject(GameObject* _obj, int Layer)
	{
		// �����̃��C���[�ɃI�u�W�F�N�g�̖��O�Ƃ��ēo�^
		mSceneObjectList[Layer].emplace_back(_obj);
		mKeyObjectList.AddGameObject(_obj);
	}
	// �o�^�I�u�W�F�N�g�̎擾(GetGameObject��������)
	GameObject* GetKeyObject(const char* _name)
	{
		return mKeyObjectList.GetGameObject(_name);
	}
	GameObject* GetKeyObject(std::string _name)
	{
		return mKeyObjectList.GetGameObject(_name);
	}

	// �ʏ�̃I�u�W�F�N�g�擾(for���ŉ񂵂Ă��镪�����͒x���Ȃ�)
	GameObject* GetGameObject(const char* _name)
	{
		for (auto& List : mSceneObjectList)
		{
			for (auto& obj : List)
			{
				if (obj->sObjectName == _name)
				{
					return obj;
				}
			}
		}
		return nullptr;
	}
	GameObject* GetGameObject(std::string _name)
	{
		for (auto& List : mSceneObjectList)
		{
			for (auto& obj : List)
			{
				if (obj->sObjectName == _name)
				{
					return obj;
				}
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> ret;
		for (auto& List : mSceneObjectList)
		{
			for (auto& obj : List)
			{
				if (typeid(*obj) == typeid(T))
				{
					ret.push_back(static_cast<T*>(obj));
				}
			}
		}

		return ret;
	}
	template<typename T>
	std::vector<T*>GetGameObjects(int _layer)
	{
		std::vector<T*>ret;
		for (auto& obj : mSceneObjectList[_layer])
		{
			if (typeid(*obj) == typeid(T))
			{
				ret.push_back(static_cast<T*>(obj));
			}
		}
		return ret;
	}


	std::string GetSceneName();
};