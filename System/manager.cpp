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
	// タイマ解像度を１ミリ秒にする
	timeBeginPeriod(1);

	// ビューリストの初期化
	DirectViewList::Init();

	// マテリアルプールの初期化
	MaterialPool::Init();

	// レンダラーを初期化
	Renderer::Init(ap);

	// ImGuiの初期化
	GUI::InitImGui(ap->GetWindow(), Renderer::GetDevice(), Renderer::GetDeviceContext());


	// ジオメトリ
	Geometory::Create();

	Com_Audio::InitMaster();
	// マスターボリューム設定
	const float InitMasterVolume = 0.5f;
	Com_Audio::SetMasterVolume(InitMasterVolume);

	// 乱数初期化
	void RandInit();

	// 高性能乱数初期化
	HighRandom->HighRandInit();

	// コリジョンマネージャー初期化
	CollisionManager::Init();

	// テクスチャ作成クラスの初期化
	TextureCreate::GetInstance();

	// Input初期化
	Input::Init();

	// タイムの初期化
	Time->Init();

	

	SetScene<Scene_ShaderLoad>();
}

void Manager::Uninit()
{
	p_mCurrentScene->UninitBase();
	delete p_mCurrentScene;

	// 時間管理オブジェクト解放
	Time->Uninit();
	// 高精度乱数解放
	HighRandom->Uninit();
	// オーディオ解放
	Com_Audio::UninitMaster();
	// テクスチャ作成オブジェクト解放
	TextureCreate::Uninit();

	// 当たり判定マネージャー解放
	CollisionManager::Uninit();

	// シェーダー解放
	ShaderPool::Uninit();
	// スタティックメッシュ解放
	StaticModelPool::Uninit();
	// スケルタルメッシュ解放
	SkeletalModelPool::Uninit();
	// ライト解放
	LightPool::Uninit();
	// PBRマテリアル解放
	MaterialPool::Uninit();

	// ジオメトリ
	Geometory::Uninit();

	// ImGuiの解放
	GUI::UninitImGui();

	// レンダラー終了処理
	Renderer::Uninit();

	// ビューリスト削除
	DirectViewList::Uninit();

	// タイマ解像度を元に戻す
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

	// 入力更新
	Input::Update();
	// コントローラー入力更新
	Controller_Input::UpdateCurrentController();
	// ImGui更新
	GUI::UpdateImGui();
	// シーンアップデート
	p_mCurrentScene->UpdateBase();

	p_mCurrentScene->GUIDraw();
	
	// マウスのリセット
	Input::RefreshMouse();
}

void Manager::Draw(uint64_t d)
{
	Renderer::Begin();

	p_mCurrentScene->DrawBase();
	GUI::RenderImGui();

	Renderer::End();
}
