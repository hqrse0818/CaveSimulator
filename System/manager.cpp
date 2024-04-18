#include	"../DirectX/renderer.h"
#include	"manager.h"
#include "../Component/Com_Audio.h"
#include	"TextureCreate.h"
#include	"../Scene/Scene.h"
#include	"Input.h"
#include	"Time.h"
#include	"../Scene/Scene_ModelLoad.h"
#include "../Scene/Scene_ShaderLoad.h"
#include "HighGetRand.h"
#include "SkeletalModelPool.h"
#include "StaticModelPool.h"
#include "ShaderPool.h"
#include "CollisionManager.h"
#include "../DirectX/DirectViewList.h"
#include "LightPool.h"
#include "MaterialPool.h"
#include "../DirectX/Geometory.h"
#include <thread>
#include "SolutionCSV.h"
#include "imguiManager.h"

using namespace std;

Scene* Manager::p_mCurrentScene{};
Scene* Manager::p_mNextScene{};
bool Manager::bChangeScene = false;

void Manager::Init(Application* ap)
{
	// �^�C�}�𑜓x���P�~���b�ɂ���
	timeBeginPeriod(1);

	// �r���[���X�g�̏�����
	DirectViewList::Init();

	// �}�e���A���v�[���̏�����
	MaterialPool::Init();

	// �����_���[��������
	Renderer::Init(ap);

	// ImGui�̏�����
	GUI::InitImGui(ap->GetWindow(), Renderer::GetDevice(), Renderer::GetDeviceContext());


	// �W�I���g��
	Geometory::Create();

	Com_Audio::InitMaster();
	// �}�X�^�[�{�����[���ݒ�
	const float InitMasterVolume = 0.5f;
	Com_Audio::SetMasterVolume(InitMasterVolume);

	// ����������
	void RandInit();

	// �����\����������
	HighRandom->HighRandInit();

	// �R���W�����}�l�[�W���[������
	CollisionManager::Init();

	// �e�N�X�`���쐬�N���X�̏�����
	TextureCreate::GetInstance();

	// Input������
	Input::Init();

	// �^�C���̏�����
	Time->Init();

	

	SetScene<Scene_ShaderLoad>();
}

void Manager::Uninit()
{
	p_mCurrentScene->UninitBase();
	delete p_mCurrentScene;

	// ���ԊǗ��I�u�W�F�N�g���
	Time->Uninit();
	// �����x�������
	HighRandom->Uninit();
	// �I�[�f�B�I���
	Com_Audio::UninitMaster();
	// �e�N�X�`���쐬�I�u�W�F�N�g���
	TextureCreate::Uninit();

	// �����蔻��}�l�[�W���[���
	CollisionManager::Uninit();

	// �V�F�[�_�[���
	ShaderPool::Uninit();
	// �X�^�e�B�b�N���b�V�����
	StaticModelPool::Uninit();
	// �X�P���^�����b�V�����
	SkeletalModelPool::Uninit();
	// ���C�g���
	LightPool::Uninit();
	// PBR�}�e���A�����
	MaterialPool::Uninit();

	// �W�I���g��
	Geometory::Uninit();

	// ImGui�̉��
	GUI::UninitImGui();

	// �����_���[�I������
	Renderer::Uninit();

	// �r���[���X�g�폜
	DirectViewList::Uninit();

	// �^�C�}�𑜓x�����ɖ߂�
	timeEndPeriod(1);
}

void Manager::Update(uint64_t d)
{
	if (bChangeScene)
	{
		p_mCurrentScene->UninitBase();
		delete p_mCurrentScene;

		p_mCurrentScene = p_mNextScene;
		p_mCurrentScene->InitBase();
		p_mCurrentScene->StartBase();
		bChangeScene = false;
		p_mNextScene = nullptr;
	}

	// ���͍X�V
	Input::Update();
	// �R���g���[���[���͍X�V
	Controller_Input::UpdateCurrentController();
	// ImGui�X�V
	GUI::UpdateImGui();
	// �V�[���A�b�v�f�[�g
	p_mCurrentScene->UpdateBase();

	p_mCurrentScene->GUIDraw();
	
	// �}�E�X�̃��Z�b�g
	Input::RefreshMouse();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();

	p_mCurrentScene->DrawBase();
	GUI::RenderImGui();

	Renderer::End();
}
