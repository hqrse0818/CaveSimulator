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
// レイヤー登録概要
// ゲームベース↓
// 0 カメラなど優先順位最高
// 1 プレイヤーや敵などのMovableで当たり判定を持つもの
// 2 固定オブジェクトで当たり判定を持つもの
// 3 その他オブジェクト
// 5 UIスプライト当たり判定のないもの
// 6  UIスプライトMovableたり判定のあるもの
// 7 UIスプライト固定当たり判定のあるもの
// 8 UIスプライトその他


#define RenderingLayerNum 6
// ディファードレンダリングベース↓
// 描画手順で整列(コンポーネントベース)
// 0 カメラの描画(WVP変換行列の定数バッファ更新)
#define ObjListIndex_Camera 0
// 1 静的オブジェクト
#define ObjListIndex_Static 1
// 2 動的オブジェクト
#define ObjListIndex_Skeletal 2
// 3 半透明オブジェクト
#define ObjListIndex_Transparent 3
// 4 エフェクト
#define ObjListIndex_Effect 4
// 5 UI
#define ObjListIndex_UI 5

// 実装する描画手順
// GBufferパスの実行(1, 2オブジェクト)
// ↓
// ディファードレンダリング用レンダーターゲットに切り替え
// ↓
// ディファードのパス実行
// ↓
// 半透明オブジェクトやエフェクトの描画(フォワード)
// ↓
// ポストプロセス
// ↓
// UIの描画

class Scene
{
protected:// 変数

	// 配置オブジェクトやライトのデータの読み込みで使用する名前
	std::string mSceneName = "NoName";

	// 描画タイプ(ポストエフェクトなどで増やす場合はここを増やす)
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

	// 環境マッピングのレベル
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

	//	オブジェクトリスト
	std::array<Objectlist, RenderingLayerNum> mSceneObjectList;
	// レンダリングベース
	//std::array<Objectlist,LayerNum>

	// リストのサイズ
	int ListSize = 0;
	// 次のシーンのロード
	bool bLoadNextScene = false;

	// 主要オブジェクトリスト(unorderd_mapで管理取得が高速)
	ObjectManager mKeyObjectList;

	// シーンで当たり判定を行うか
	bool bCheckCol = false;

	GameObject* pEmpty{};
	Com_SimpleTranslate* pTransCom{};

	// ベースシーンにおけるカメラとターゲットオブジェクトの操作
	bool bOperateCamera = true;

	GameObject* pCamera{};
	Com_Camera* pMainCamera{};
	float fAngleSpeed = 1.0f;

	// 描画タイプ
	SceneDrawType mDrawType = SceneDrawType::Deffered;
	GBufferType mBufferType = GBufferType::Main;
	EnvironmentCurrentRenderType mEnvCurrent = EnvironmentCurrentRenderType::Front;

	//GUIの表示
	bool bShowGUI = false;
	// シーンデータの表示
	bool bShowSceneData = false;
	// シーンのオブジェクトデータの表示
	bool bShowObjectData = true;
	// モデルリストの表示
	bool bShowModelData = false;
	// マテリアルデータの表示
	bool bShowMaterialData = false;
	// ライトのリストの表示
	bool bShowLightData = false;

	// GBuffer用オブジェクト
	GameObject* pNormal{};
	GameObject* pAlbedo{};
	GameObject* pWorld{};
	GameObject* pMaterial{};
	GameObject* pDeffered{};
	GameObject* pResultDef{};
	// GBuffer用スプライト
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

	// キューブマップジェネレーター
	CubeMapGenerator* pCubeMapGenerator{};

	// 環境マッピング用オブジェクト
	DynamicEnvironmentMap* p_mFrontMap{};
	DynamicEnvironmentMap* p_mBackMap{};
	DynamicEnvironmentMap* p_mRightMap{};
	DynamicEnvironmentMap* p_mLeftMap{};
	DynamicEnvironmentMap* p_mTopMap{};
	DynamicEnvironmentMap* p_mBottomMap{};
	// キューブマップ用レンダーターゲット
	RenderTarget* pRTVCubeMap{};

	// キューブマップ用オブジェクト
	GameObject* pCubeMapObj{};
	Com_Sprite* pCubeMapSprite{};

	// テストスプライト
	GameObject* p_mTestObject{};
	Com_Sprite* p_mTestSprite{};

	// 法線マップ用スプライト
	Com_Sprite* p_mCrystalNormalSprite{};

protected:// 関数
	void SetMainCamera(Com_Camera* _cam)
	{
		pMainCamera = _cam;
	}

	// 当たり判定処理の実行
	virtual void CollisionUpdate();

	// GUI
	void GuiUpdateBase();

	// シーンデータGUI
	void ShowGuiSceneData();
	// オブジェクトデータGUI
	void ShowGuiObjectData();
	// 配置したオブジェクトデータのファイル保存
	void SaveLevelData();
	// オブジェクトデータの読み込み
	void ReadLevelData();
	// ライトのデータの読み込み
	void ReadLightData();
	// マテリアルデータの読み込み
	void ReadMaterialData(const char* _FileName);
	// 環境マッピング生成関数
	void CreateEnvironmentMap();
	// 環境マップにレンダリングするオブジェクトの描画
	void RenderEnvironmentObject(DirectX::SimpleMath::Matrix _Matrix);
	// キューブマップの作成
	void CreateCubeMap();

	std::string CreateLevelFileName();
	std::string CreateLightFileName();

public:// 関数

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
	// シェーダーの読み込みなど
	virtual void PreInit() {};
	// オブジェクト登録、変数初期化などの初期化処理
	virtual void Init() {};
	// ループ開始前処理(BGMオンなど)
	virtual void Start() {};
	// 更新処理
	virtual void Update() {};
	// 描画前に行う処理
	virtual void PreDraw() {};
	// 3Dオブジェクト描画後処理
	virtual void Draw3D() {};
	// 2Dオブジェクト描画後処理
	virtual void Draw2D() {};
	// Guiの描画
	virtual void GUIDraw() {};

	virtual void MakeShadowMap();
	virtual void Uninit() {};

	// 初期化のベース処理
	void InitBase();
	// 開始のベース処理
	void StartBase()
	{
		// ゲームオブジェクトのターゲットシーンの設定
		GameObject::SetScene(this);

		// オブジェクトリストの数だけ回す
		for (auto& List : mSceneObjectList)
		{
			// リストの中身の数分解放処理
			for (auto& obj : List)
			{
				obj->StartBase();
			}
		}

		Start();
	}
	// 開放のベース処理
	void UninitBase();
	// 更新のベース処理
	void UpdateBase();
	// 描画前処理のベース処理
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
	// 描画のベース処理
	void DrawBase();

	// 通常のオブジェクト登録
	void AddGameObject(GameObject* _obj, int Layer)
	{
		mSceneObjectList[Layer].emplace_back(_obj);
	}
	// 格納配列の決定
	int LayerCheck(GameObject* _obj);
	// キーオブジェクトとして登録
	// unordered_mapに登録(取得までの処理は速いがfor文には適していない)
	
	void AddKeyObject(GameObject* _obj, int Layer)
	{
		// 引数のレイヤーにオブジェクトの名前として登録
		mSceneObjectList[Layer].emplace_back(_obj);
		mKeyObjectList.AddGameObject(_obj);
	}
	// 登録オブジェクトの取得(GetGameObjectよりも高速)
	GameObject* GetKeyObject(const char* _name)
	{
		return mKeyObjectList.GetGameObject(_name);
	}
	GameObject* GetKeyObject(std::string _name)
	{
		return mKeyObjectList.GetGameObject(_name);
	}

	// 通常のオブジェクト取得(for文で回している分処理は遅くなる)
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