#include "Scene.h"
#include "../System/Collision.h"
#include "../System/CollisionManager.h"
#include "../main.h"
#include "../DirectX/DirectViewList.h"
#include "../System/imguiManager.h"
#include "../System/FileOperation.h"
#include "../System/SolutionCSV.h"
#include "../System/StaticModelPool.h"
#include "../System/MaterialPool.h"
#include "../System/LightPool.h"
#include "../System/Time.h"


using namespace DirectX::SimpleMath;
using namespace std;

void Scene::InitBase()
{

	// 初期化前に呼ぶ処理
	PreInit();


	// カメラの作成
	pCamera = new GameObject("Camera");
	pMainCamera = new Com_AngleCamera();
	pCamera->AddComponent(pMainCamera);
	pCamera->InitBase();
	SetMainCamera(static_cast<Com_Camera*>(pMainCamera));
	AddKeyObject(pCamera, 0);

	// カメラのターゲット作成
	pEmpty = new GameObject("CameraTarget");
	pTransCom = new Com_SimpleTranslate();
	pTransCom->SetCamera(pCamera);
	pEmpty->AddComponent(pTransCom);
	pMainCamera->SetTarget(pEmpty);
	AddKeyObject(pEmpty, 1);


	// レンダーターゲットとデプスステンシルビューの取得
	{
		// GBufferの取得
		pGBufferNormal = DirectViewList::GetRTV("RTVNormal");
		pGBufferAlbedo = DirectViewList::GetRTV("RTVAlbedo");
		pGBufferWorld = DirectViewList::GetRTV("RTVWorld");
		pGBufferMaterial = DirectViewList::GetRTV("RTVMaterial");
	}

	// 描画開始位置の定義
	Vector3 InitPos = Vector3(SpriteWidth / 2, SpriteHeight / 2, 0.0f);

	// ディファードレンダリング用オブジェクトの作成
	{
		// GBuffer
		{
			// 法線描画
			pNormal = new GameObject("Normal");
			pNormalSprite = new Com_Sprite();
			pNormalSprite->SetRelease();
			pNormalSprite->SetVertex("VS_SPRITE");
			pNormalSprite->SetPixel("PS_SPRITE");
			pNormalSprite->SetSize(SpriteWidth, SpriteHeight);
			pNormalSprite->SetTexture(DirectViewList::GetRTV("RTVNormal"));
			pNormal->AddComponent(pNormalSprite);
			pNormal->SetPosition(InitPos.x, InitPos.y);
			pNormal->InitBase();
			// アルベドカラー
			pAlbedo = new GameObject("Albedo");
			pAlbedoSprite = new Com_Sprite();
			pAlbedoSprite->SetRelease();
			pAlbedoSprite->SetVertex("VS_SPRITE");
			pAlbedoSprite->SetPixel("PS_SPRITE");
			pAlbedoSprite->SetSize(SpriteWidth, SpriteHeight);
			pAlbedoSprite->SetTexture(DirectViewList::GetRTV("RTVAlbedo"));
			pAlbedo->AddComponent(pAlbedoSprite);
			pAlbedo->SetPosition(InitPos.x * 3, InitPos.y);
			pAlbedo->InitBase();
			// ワールド座標
			pWorld = new GameObject("World");
			pWorldSprite = new Com_Sprite();
			pWorldSprite->SetRelease();
			pWorldSprite->SetVertex("VS_SPRITE");
			pWorldSprite->SetPixel("PS_SPRITE");
			pWorldSprite->SetSize(SpriteWidth, SpriteHeight);
			pWorldSprite->SetTexture(DirectViewList::GetRTV("RTVWorld"));
			pWorld->AddComponent(pWorldSprite);
			pWorld->SetPosition(InitPos.x, InitPos.y * 3);
			pWorld->InitBase();
			// マテリアルID
			pMaterial = new GameObject("Material");
			pMaterialSprite = new Com_Sprite();
			pMaterialSprite->SetRelease();
			pMaterialSprite->SetVertex("VS_SPRITE");
			pMaterialSprite->SetPixel("PS_SPRITE");
			pMaterialSprite->SetSize(SpriteWidth, SpriteHeight);
			pMaterialSprite->SetTexture(DirectViewList::GetRTV("RTVMaterial"));
			pMaterial->AddComponent(pMaterialSprite);
			pMaterial->SetPosition(InitPos.x * 3, InitPos.y * 3);
			pMaterial->InitBase();
		}
		// ディファードレンダリング用
		{
			pDeffered = new GameObject("Deffered");
			pDefferedSprite = new Com_Sprite();
			pDefferedSprite->SetVertex("VS_SPRITE");
			pDefferedSprite->SetPixel("PS_DisneyPBR");
			pDeffered->AddComponent(pDefferedSprite);
			// 全画面
			pDefferedSprite->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
			pDeffered->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			pDeffered->InitBase();
		}
		// ディファードレンダリングの結果格納用
		{
			pResultDef = new GameObject("ResultDeffered");
			pResultDefSprite = new Com_Sprite();
			pResultDefSprite->SetRelease();
			pResultDefSprite->SetVertex("VS_SPRITE");
			pResultDefSprite->SetPixel("PS_SPRITE");
			pResultDefSprite->SetSize(MAX_SPRITE_WIDTH, MAX_SPRITE_HEIGHT);
			pResultDefSprite->SetTexture(DirectViewList::GetRTV("RTVDeffered"));
			pResultDef->AddComponent(pResultDefSprite);
			pResultDef->SetPosition(InitPos.x * 2, InitPos.y * 2);
			pResultDef->InitBase();
		}
	}

	// 環境マップ用オブジェクト
	// 正面
	p_mFrontMap = new DynamicEnvironmentMap("FrontMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mFrontMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mFrontMap->InitObject();
	// 背面
	p_mBackMap = new DynamicEnvironmentMap("BackMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mBackMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mBackMap->InitObject();
	// 右面
	p_mRightMap = new DynamicEnvironmentMap("RightMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mRightMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mRightMap->InitObject();
	// 左面
	p_mLeftMap = new DynamicEnvironmentMap("LeftMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mLeftMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mLeftMap->InitObject();
	// 上面
	p_mTopMap = new DynamicEnvironmentMap("TopMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mTopMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mTopMap->InitObject();
	// 下面
	p_mBottomMap = new DynamicEnvironmentMap("BottomMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mBottomMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mBottomMap->InitObject();
	// キューブマップ用レンダーターゲット
	pRTVCubeMap = new RenderTarget();
	pRTVCubeMap->Create(DXGI_FORMAT_R16G16B16A16_FLOAT, ENVIRONMENT_CUBEMAP_WIDTH, ENVIRONMENT_CUBEMAP_HEIGHT);

	pCubeMapObj = new GameObject("CubeMap");
	pCubeMapSprite = new Com_Sprite();
	pCubeMapSprite->SetRelease();
	pCubeMapSprite->SetUpdate(true);
	pCubeMapSprite->SetVertex("VS_SPRITE");
	pCubeMapSprite->SetPixel("PS_SPRITE");
	pCubeMapSprite->SetSize(300.0f, 300.0f);
	pCubeMapObj->AddComponent(pCubeMapSprite);
	pCubeMapObj->InitBase();

	p_mTestObject = new GameObject("Test");
	p_mTestSprite = new Com_Sprite();
	p_mTestSprite->SetTexture("asset/texture/LightBaseTexture.png");
	p_mTestSprite->SetVertex("VS_SPRITE");
	p_mTestSprite->SetPixel("PS_SPRITE");
	p_mTestSprite->SetUpdate(true);
	p_mTestObject->AddComponent(p_mTestSprite);
	p_mTestSprite->SetSize(200.0f, 200.0f);
	p_mTestObject->SetPosition(400.0f, 400.0f);
	p_mTestObject->InitBase();

	// キューブマップ作製用
	pCubeMapGenerator = new CubeMapGenerator();
	pCubeMapGenerator->Init(ENVIRONMENT_CUBEMAP_HEIGHT, ENVIRONMENT_CUBEMAP_HEIGHT);

	// 自身の初期化処理を呼ぶ
	Init();

	for (auto& List : mSceneObjectList)
	{
		for (auto& obj : List)
		{
			obj->InitBase();
		}
	}
}

void Scene::UninitBase()
{
	// GBuffer用,ライティング用オブジェクトの解放
	pResultDef->Uninit();
	SAFE_NULL(pResultDef);
	pDeffered->Uninit();
	SAFE_NULL(pDeffered);
	pMaterial->Uninit();
	SAFE_NULL(pMaterial);
	pWorld->Uninit();
	SAFE_NULL(pWorld);
	pAlbedo->Uninit();
	SAFE_NULL(pAlbedo);
	pNormal->Uninit();
	SAFE_NULL(pNormal);

	// 環境マップオブジェクトの解放
	p_mFrontMap->Uninit();
	SAFE_NULL(p_mFrontMap);
	p_mBackMap->Uninit();
	SAFE_NULL(p_mBackMap);
	p_mRightMap->Uninit();
	SAFE_NULL(p_mRightMap);
	p_mLeftMap->Uninit();
	SAFE_NULL(p_mLeftMap);
	p_mTopMap->Uninit();
	SAFE_NULL(p_mTopMap);
	p_mBottomMap->Uninit();
	SAFE_NULL(p_mBottomMap);

	// キューブマップのオブジェクトの解放
	pCubeMapObj->Uninit();
	SAFE_NULL(pCubeMapObj);
	pRTVCubeMap->Uninit();
	SAFE_NULL(pRTVCubeMap);
	pCubeMapGenerator->Uninit();
	SAFE_NULL(pCubeMapGenerator);


	p_mTestObject->Uninit();
	SAFE_NULL(p_mTestObject);
	
	LightPool::ReleasePointLight();
	LightPool::ReleaseSpotLight();

	// オブジェクトリストの数だけ回す
	for (auto& List : mSceneObjectList)
	{
		// リストの中身の数分解放処理
		for (auto& obj : List)
		{
			obj->Uninit();
			delete obj;
		}
		List.clear();
	}

	// 自身の解放処理を呼ぶ
	Uninit();
}

void Scene::UpdateBase()
{
	bFullScreenOld = bFullScreen;

	// 更新処理(List仕様)
	for (auto& List : mSceneObjectList)
	{
		for (auto& obj : List)
		{
			obj->UpdateBase();
		}
		// ラムダ式による破壊チェック
		List.remove_if([](GameObject* object)
		{
			bool des = object->Destroy();
			if (des)
			{
				object = nullptr;
			}
			return des;
		});
	}

	if (bOperateCamera)
	{
		float MoveX = 0.0f;
		float MoveZ = 0.0f;
		//	ターゲットの移動
		if (Input::GetKeyState(KEYCODE_W) == KEYSTATE::KEY_WHILE_DOWN)
		{
			MoveZ = 1.0f;
		}
		else if (Input::GetKeyState(KEYCODE_S) == KEYSTATE::KEY_WHILE_DOWN)
		{
			MoveZ = -1.0f;
		}
		if (Input::GetKeyState(KEYCODE_A) == KEYSTATE::KEY_WHILE_DOWN)
		{
			MoveX = -1.0f;
		}
		else if (Input::GetKeyState(KEYCODE_D) == KEYSTATE::KEY_WHILE_DOWN)
		{
			MoveX = 1.0f;
		}

		if (abs(Controller_Input::GetLeftStick(0).x) > 0.3f)
		{
			MoveX = Controller_Input::GetLeftStick(0).x;
		}
		if (abs(Controller_Input::GetLeftStick(0).y) > 0.3f)
		{
			MoveZ = Controller_Input::GetLeftStick(0).y;
		}
		pTransCom->Move(MoveZ, MoveX);

		Com_AngleCamera* cam = static_cast<Com_AngleCamera*>(pMainCamera);
		// カメラの回転
		if (Input::GetKeyState(KEYCODE_UP) == KEYSTATE::KEY_WHILE_DOWN)
		{
			cam->SetHeight(cam->GetHeight() + (fAngleSpeed * Time->GetDeltaTime()));
		}
		else if (Input::GetKeyState(KEYCODE_DOWN) == KEYSTATE::KEY_WHILE_DOWN)
		{
			cam->SetHeight(cam->GetHeight() - (fAngleSpeed * Time->GetDeltaTime()));
		}
		if (Input::GetKeyState(KEYCODE_RIGHT) == KEYSTATE::KEY_WHILE_DOWN)
		{
			cam->SetAngle(cam->GetAngle() - (fAngleSpeed * Time->GetDeltaTime()));
		}
		else if (Input::GetKeyState(KEYCODE_LEFT) == KEYSTATE::KEY_WHILE_DOWN)
		{
			cam->SetAngle(cam->GetAngle() + (fAngleSpeed * Time->GetDeltaTime()));
		}

		if (abs(Controller_Input::GetRightStick(0).x) > 0.3f)
		{
			cam->SetAngle(cam->GetAngle() + (fAngleSpeed * Time->GetDeltaTime()) * (Controller_Input::GetRightStick(0).x > 0.0f ? 1.0f : -1.0f));
		}
		if (abs(Controller_Input::GetRightStick(0).y) > 0.3f)
		{
			cam->SetHeight(cam->GetHeight() + (fAngleSpeed * Time->GetDeltaTime() * (Controller_Input::GetRightStick(0).y > 0.0f ? 1.0f : -1.0f)));
		}
	}
	

	// 自身の更新処理を呼ぶ
	Update();

	if (bCheckCol == true)
	{
		CollisionUpdate();
	}

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);
	ImGui::Begin("Option");
	ImGui::Checkbox("ShowGUI", &bShowGUI);
	if (bShowGUI)
	{
		GuiUpdateBase();
	}
	ImGui::End();

	// スクリーンサイズの調整
	if (bFullScreen && !bFullScreenOld)
	{
		pResultDefSprite->SetSize(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
		pResultDef->SetPosition(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
		pResultDefSprite->SetUpdate(true);
		pResultDefSprite->Update();
		pResultDefSprite->SetUpdate(false);
	}
	else if (!bFullScreen && bFullScreenOld)
	{
		pResultDefSprite->SetSize(static_cast<float>(MAX_SPRITE_WIDTH), static_cast<float>(MAX_SPRITE_HEIGHT));
		pResultDef->SetPosition(static_cast<float>(MAX_SPRITE_WIDTH / 2), static_cast<float>(MAX_SPRITE_HEIGHT / 2));
		pResultDefSprite->SetUpdate(true);
		pResultDefSprite->Update();
		pResultDefSprite->SetUpdate(false);
	}

	// 定数バッファ更新
	Renderer::SetCameraPos(pMainCamera->p_mObject->GetPosition());
	LightPool::UpdateDirectionalLight();
	LightPool::UpdateSpotLights();
	LightPool::UpdatePointLights();

}

void Scene::DrawBase()
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	// 描画先をGBufferに切り替える
	// GBuffer用レンダーターゲット
	RenderTarget* GBuffer[] =
	{
		pGBufferNormal,
		pGBufferAlbedo,
		pGBufferWorld,
		pGBufferMaterial
	};

	GBuffer[0]->Clear();
	GBuffer[1]->Clear();
	GBuffer[2]->Clear();
	GBuffer[3]->Clear();
	DepthStencil* DSV = DirectViewList::GetDSV("DSV");

	// 描画先変更
	Renderer::SetRenderTargets(4, GBuffer, DSV);
	// 深度テスト
	Renderer::SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	// ベースとなるワールド変換行列
	DirectX::SimpleMath::Matrix matrix;
	matrix = DirectX::SimpleMath::Matrix::Identity;

	// GBufferパスの実行
	// カメラの描画(WVP変換行列の定数バッファセット)
	for (auto& obj : mSceneObjectList[ObjListIndex_Camera])
	{
		obj->DrawBase(matrix);
	}
	// 静的オブジェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Static])
	{
		obj->DrawBase(matrix);
	}
	// 動的オブジェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Skeletal])
	{
		obj->DrawBase(matrix);
	}

	// シャドウパス


	// 環境マップパス
	// 動的環境マップの生成
	//CreateEnvironmentMap();
	// 確認用キューブマップの生成
	//CreateCubeMap();

	RenderTarget* EnvironmentMaps[] = {
		p_mRightMap->GetDefferedTexture(),
		p_mLeftMap->GetDefferedTexture(),
		p_mTopMap->GetDefferedTexture(),
		p_mBottomMap->GetDefferedTexture(),
		p_mFrontMap->GetDefferedTexture(),
		p_mBackMap->GetDefferedTexture()
	};

	//pCubeMapGenerator->Generate(EnvironmentMaps);

	// ディファードパスの実行
	RenderTarget* DefferdRTV = DirectViewList::GetRTV("RTVDeffered");
	DefferdRTV->Clear(ClearColor);
	Renderer::SetRenderTargets(1, &DefferdRTV, nullptr);

	// GBufferをテクスチャとしてセット
	switch (mDrawType)
	{
	case Scene::SceneDrawType::GBuffer:
		break;
	case Scene::SceneDrawType::Deffered:
		pDefferedSprite->SetPixel("PS_DisneyPBR");
		//pDefferedSprite->SetPixel("PS_ToonPBR");
		pDefferedSprite->GetPS()->SetTexture(1, GBuffer[0]);
		pDefferedSprite->GetPS()->SetTexture(2, GBuffer[1]);
		pDefferedSprite->GetPS()->SetTexture(3, GBuffer[2]);
		pDefferedSprite->GetPS()->SetTexture(4, GBuffer[3]);
		pDefferedSprite->GetPS()->SetTexture(5, pCubeMapGenerator->GetCubeMap());
		pDeffered->DrawBase(matrix);
		break;
	case Scene::SceneDrawType::NoTexture:
		break;
	case SceneDrawType::DefferedPoint:
		break;
	case SceneDrawType::OnlyEnvironment:
		break;
	case SceneDrawType::CubeMap:
		break;
	case Scene::SceneDrawType::Max:
		break;
	default:
		pDefferedSprite->SetPixel("PS_DisneyPBR");
		pDefferedSprite->GetPS()->SetTexture(1, GBuffer[0]);
		pDefferedSprite->GetPS()->SetTexture(2, GBuffer[1]);
		pDefferedSprite->GetPS()->SetTexture(3, GBuffer[2]);
		pDefferedSprite->GetPS()->SetTexture(4, GBuffer[3]);
		pDefferedSprite->GetPS()->SetTexture(5, pCubeMapGenerator->GetCubeMap());
		pDeffered->DrawBase(matrix);
		break;
	}
	

	// レンダーターゲットをディファード用に設定
	Renderer::SetRenderTargets(1, &DefferdRTV, DSV);

	// 半透明オブジェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Transparent])
	{
		obj->DrawBase(matrix);
	}
	// エフェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Effect])
	{
		obj->DrawBase(matrix);
	}

	// 3D描画後のシーンの処理
	Draw3D();

	// レンダーターゲットをバックバッファに指定
	RenderTarget* BackBuffer = DirectViewList::GetRTV("BackBuffer");
	Renderer::SetRenderTargets(1, &BackBuffer, nullptr);

	switch (mDrawType)
	{
	case Scene::SceneDrawType::GBuffer:
		// GBufferの描画
		switch (mBufferType)
		{
		case Scene::GBufferType::Main:
			pNormalSprite->SetTexture(GBuffer[0]);
			pAlbedoSprite->SetTexture(GBuffer[1]);
			pWorldSprite->SetTexture(GBuffer[2]);
			pMaterialSprite->SetTexture(GBuffer[3]);
			break;
		case Scene::GBufferType::EnvFront:
			pNormalSprite->SetTexture(p_mFrontMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mFrontMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mFrontMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mFrontMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::EnvBack:
			pNormalSprite->SetTexture(p_mBackMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mBackMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mBackMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mBackMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::EnvRight:
			pNormalSprite->SetTexture(p_mRightMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mRightMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mRightMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mRightMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::EnvLeft:
			pNormalSprite->SetTexture(p_mLeftMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mLeftMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mLeftMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mLeftMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::EnvTop:
			pNormalSprite->SetTexture(p_mTopMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mTopMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mTopMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mTopMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::EnvBottom:
			pNormalSprite->SetTexture(p_mBottomMap->GetNormalRTV());
			pAlbedoSprite->SetTexture(p_mBottomMap->GetAlbedoRTV());
			pWorldSprite->SetTexture(p_mBottomMap->GetWorldRTV());
			pMaterialSprite->SetTexture(p_mBottomMap->GetMaterialRTV());
			break;
		case Scene::GBufferType::Max:
			break;
		default:
			break;
		}
		pNormal->Draw();
		pAlbedo->Draw();
		pWorld->Draw();
		pMaterial->Draw();
		break;
	case Scene::SceneDrawType::Deffered:
		pResultDefSprite->SetTexture(DefferdRTV);
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvFront:
		pResultDefSprite->SetTexture(p_mFrontMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvBack:
		pResultDefSprite->SetTexture(p_mBackMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvRight:
		pResultDefSprite->SetTexture(p_mRightMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvLeft:
		pResultDefSprite->SetTexture(p_mLeftMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvTop:
		pResultDefSprite->SetTexture(p_mTopMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case SceneDrawType::EnvBottom:
		pResultDefSprite->SetTexture(p_mBottomMap->GetDefferedTexture());
		pResultDef->DrawBase(matrix);
		break;
	case Scene::SceneDrawType::Max:
		break;
	default:
		break;
	}
	
	// UIを描画
	for (auto& obj : mSceneObjectList[ObjListIndex_UI])
	{
		obj->DrawBase(matrix);
	}

	// 2D描画後の処理
	Draw2D();
}


void Scene::CreateEnvironmentMap()
{
	// ベースとなるワールド変換行列
	DirectX::SimpleMath::Matrix matrix;
	matrix = DirectX::SimpleMath::Matrix::Identity;

	// メインカメラのポジションを取得
	Vector3 CenterPosition = pMainCamera->p_mObject->GetPosition();

	Vector3 Up = pMainCamera->GetUp();
	// 環境マップ生成時の注視点
	Vector3 TargetPosition;
	// 環境マップ用プロジェクション変換行列
	float aspect = static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT) / static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT);
	//float Fov = 90.0f;
	float Fov = 90.0f;
	float Near = 1.0f;
	float Far = 200.0f;
	Matrix Proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(Fov), aspect, Near, Far);
	Renderer::SetProjectionMatrix(&Proj);

	int drawtype = static_cast<int>(mEnvCurrent);
	mEnvCurrent = static_cast<EnvironmentCurrentRenderType>(drawtype+ 1);
	if (mEnvCurrent == EnvironmentCurrentRenderType::Max)
	{
		mEnvCurrent = EnvironmentCurrentRenderType::Front;
	}

	// 6フレームに1回各方向の環境マップが更新される
	switch (mEnvCurrent)
	{
	case Scene::EnvironmentCurrentRenderType::Front:
	{
		// 正面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.z += 1.0f;
		// 正面方向のビューマトリックスの生成
		Matrix FrontView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		// ビューマトリックスの定数バッファをセット
		Renderer::SetViewMatrix(&FrontView);
		// ディファードレンダリングで使用するGBufferをレンダーターゲットに設定する
		p_mFrontMap->SetGBuffer();
		// 環境マップに描画するオブジェクトの描画
		RenderEnvironmentObject(matrix);
		// ディファードレンダリングを実行して描画結果を正面用の環境マップに格納
		p_mFrontMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Back:
	{
		// 背面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.z -= 1.0f;
		Matrix BackView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		Renderer::SetViewMatrix(&BackView);
		p_mBackMap->SetGBuffer();
		RenderEnvironmentObject(matrix);
		p_mBackMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Right:
	{
		// 右面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.x += 1.0f;
		Matrix RightView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		Renderer::SetViewMatrix(&RightView);
		p_mRightMap->SetGBuffer();
		RenderEnvironmentObject(matrix);
		p_mRightMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Left:
	{
		// 左面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.x -= 1.0f;
		Matrix LeftView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		Renderer::SetViewMatrix(&LeftView);
		p_mLeftMap->SetGBuffer();
		RenderEnvironmentObject(matrix);
		p_mLeftMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Top:
	{
		// 上面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.y += 1.0f;
		Up = Vector3::Backward;
		Matrix TopView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		Renderer::SetViewMatrix(&TopView);
		p_mTopMap->SetGBuffer();
		RenderEnvironmentObject(matrix);
		p_mTopMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Bottom:
	{
		// 下面方向の環境マップ生成
		TargetPosition = CenterPosition;
		TargetPosition.y -= 1.0f;
		Up = Vector3::Forward;
		Matrix BottomView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		Renderer::SetViewMatrix(&BottomView);
		p_mBottomMap->SetGBuffer();
		RenderEnvironmentObject(matrix);
		p_mBottomMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Max:
		break;
	default:
		break;
	}
}

void Scene::RenderEnvironmentObject(DirectX::SimpleMath::Matrix _Matrix = DirectX::SimpleMath::Matrix::Identity)
{
	// 静的オブジェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Static])
	{
		obj->DrawBase(_Matrix);
	}
	// 動的オブジェクトの描画
	for (auto& obj : mSceneObjectList[ObjListIndex_Skeletal])
	{
		obj->DrawBase(_Matrix);
	}
}

void Scene::CreateCubeMap()
{
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	pRTVCubeMap->Clear(color);
	Renderer::SetRenderTargets(1, &pRTVCubeMap, nullptr);
	// 左面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->Update();
	pCubeMapSprite->SetTexture(p_mLeftMap->GetDefferedTexture());
	pCubeMapObj->Draw();
	// 正面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 3), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mFrontMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// 右面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 5), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mRightMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// 背面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 7), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mBackMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// 上面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 3), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/6));
	pCubeMapSprite->SetTexture(p_mTopMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// 下面
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 3), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT /6 *5));
	pCubeMapSprite->SetTexture(p_mBottomMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
}

std::string Scene::CreateLevelFileName()
{
	string filename = "asset/data/csv/";
	filename += mSceneName;
	filename += "LevelData.csv";
	return filename;
}

std::string Scene::CreateLightFileName()
{
	string filename = "asset/data/csv/";
	filename += mSceneName;
	filename += "LightData.csv";
	return filename;
}

void Scene::MakeShadowMap()
{
	Matrix mat;
	mat = Matrix::Identity;

	for (auto list : mSceneObjectList)
	{
		for (auto obj : list)
		{
			obj->ShadowDrawBase(mat);
		}
	}
}


void Scene::CollisionUpdate()
{
	if (bCheckCol)
	{
		CollisionManager::ExecuteCollision();
	}
}

void Scene::GuiUpdateBase()
{
	// ImGuiによるシーンの切り替え
	ImGui::SetNextWindowPos(ImVec2(980.0f, 0.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(520.0f, 120.0f), ImGuiCond_Once);
	ImGui::Begin("Rendering");
	if (ImGui::CollapsingHeader("GBuffer"))
	{
		if (ImGui::Button("MainMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::Main;
		}
		ImGui::SameLine();
		if (ImGui::Button("FrontMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvFront;
		}
		ImGui::SameLine();
		if (ImGui::Button("BackMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvBack;
		}
		if (ImGui::Button("RightMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvRight;
		}
		ImGui::SameLine();
		if (ImGui::Button("LeftMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvLeft;
		}
		ImGui::SameLine();
		if (ImGui::Button("TopMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvTop;
		}
		ImGui::SameLine();
		if (ImGui::Button("BottomMap"))
		{
			mDrawType = SceneDrawType::GBuffer;
			mBufferType = GBufferType::EnvBottom;
		}
	}
	if (ImGui::CollapsingHeader("MainResult"))
	{
		if (ImGui::Button("Deffered"))
		{
			mDrawType = SceneDrawType::Deffered;
		}
	}
	if (ImGui::CollapsingHeader("EnvironmentMap"))
	{
		if (ImGui::Button("EnvFront"))
		{
			mDrawType = SceneDrawType::EnvFront;
		}
		ImGui::SameLine();
		if (ImGui::Button("EnvBack"))
		{
			mDrawType = SceneDrawType::EnvBack;
		}
		ImGui::SameLine();
		if (ImGui::Button("EnvRight"))
		{
			mDrawType = SceneDrawType::EnvRight;
		}
		if (ImGui::Button("EnvLeft"))
		{
			mDrawType = SceneDrawType::EnvLeft;
		}
		ImGui::SameLine();
		if (ImGui::Button("EnvTop"))
		{
			mDrawType = SceneDrawType::EnvTop;
		}
		ImGui::SameLine();
		if (ImGui::Button("EnvBottom"))
		{
			mDrawType = SceneDrawType::EnvBottom;
		}
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(MAX_SPRITE_WIDTH, 120.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(SCREEN_WIDTH - MAX_SPRITE_WIDTH), 80.0f), ImGuiCond_Once);
	ImGui::Begin("Menu");

	ImGui::Checkbox("SceneData", &bShowSceneData);
	ImGui::SameLine();
	ImGui::Checkbox("ObjectList", &bShowObjectData);
	ImGui::SameLine();
	ImGui::Checkbox("ModelList", &bShowModelData);
	ImGui::Checkbox("MaterialList", &bShowMaterialData);
	ImGui::SameLine();
	ImGui::Checkbox("LightData", &bShowLightData);
	ImGui::SameLine();
	ImGui::Checkbox("UseInput", &bOperateCamera);
	ImGui::SameLine();
	ImGui::Checkbox("FullScreen", &bFullScreen);

	// シーン内の概要データ描画
	if (bShowSceneData)
	{
		ShowGuiSceneData();
	}
	// シーン内のオブジェクトデータ描画
	if (bShowObjectData)
	{
		ShowGuiObjectData();
	}
	// モデルデータリストの表示
	if (bShowModelData)
	{
		StaticModelPool::ShowStaticModelGui();
	}
	// マテリアルデータリストの表示
	if (bShowMaterialData)
	{
		MaterialPool::ShowMaterialGui();
	}
	// ライトのデータリストの表示
	if (bShowLightData)
	{
		LightPool::ShowLightGui();
	}

	ImGui::End();

	GUI::ShowLog();
}

void Scene::ShowGuiSceneData()
{
	GUI::ShowFrameRate(0.0f, 55.0f);

	int objNum = 0;
	for (auto list : mSceneObjectList)
	{
		objNum += list.size();
	}

	ImGui::SetNextWindowPos(ImVec2(100.0f, 0.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 80.0f), ImGuiCond_Once);

	ImGui::Begin("Performance");
	// シーン内のゲームオブジェクトの数
	ImGui::Text("ObjectNum : %d", objNum);
	int staticmodelnum = Com_StaticMesh::GetModelNum();
	int skeletalmodelnum = Com_SkeletalMesh::GetModelNum();
	int modelnum = staticmodelnum + skeletalmodelnum;
	// モデルの数
	ImGui::Text("ModelNum : %d", modelnum);
	int staticvertices = Com_StaticMesh::GetVertexNum();
	int skeletalvertices = Com_SkeletalMesh::GetVertexNum();
	int totalvertices = staticvertices + skeletalvertices;
	// モデルの合計頂点数
	ImGui::Text("TotalVertices : %d", totalvertices);

	ImGui::End();
}

void Scene::ShowGuiObjectData()
{
	ImGui::SetNextWindowPos(ImVec2(MAX_SPRITE_WIDTH, 200.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(SCREEN_WIDTH - MAX_SPRITE_WIDTH), 300.0f), ImGuiCond_Once);
	ImGui::Begin("SceneData");

	// レベルのセーブ
	if (ImGui::Button("SaveLevel"))
	{
		SaveLevelData();
		GUI::PushLog("Saved Level");
	}

	ImGui::SliderFloat("CameraSpeed", &fAngleSpeed, 0.0f, 50.0f);

	static bool bSlider = false;
	ImGui::Checkbox("Slider", &bSlider);

	constexpr float PositionMax = 200.0f;
	constexpr float RotationMax = 3.14f;
	constexpr float ScaleMax = 100.0f;
	// シーン内のオブジェクトデータの表示(Transformや名前のみ)
	for (int i = 0; i < mSceneObjectList.size(); i++)
	{
		string ListName = "List";
		ListName = ListName + to_string(i);
		if (ImGui::TreeNode(ListName.c_str()))
		{
			int index = 0;
			for (auto obj : mSceneObjectList[i])
			{
				string Header = obj->sObjectName;
				Header += to_string(index);
				if (ImGui::TreeNode(Header.c_str()))
				{
					string Pos = "List" + to_string(i) + "Position_" + to_string(index);
					string Rot = "List" + to_string(i) + "Rotation_" + to_string(index);
					string Sca = "List" + to_string(i) + "Scale_" + to_string(index);
					Header += " : name";
					static char ObjectName[] = "ExampleObjectName";
					ImGui::InputText(Header.c_str(), ObjectName, sizeof(ObjectName));
					if (ImGui::Button("ChangeName"))
					{
						obj->sObjectName = ObjectName;
					}

					if (!bSlider)
					{
						ImGui::InputFloat3(Pos.c_str(), (float*)&(obj->p_mTransform->mPosition));
						ImGui::InputFloat3(Rot.c_str(), (float*)&(obj->p_mTransform->mRotation));
						ImGui::InputFloat3(Sca.c_str(), (float*)&(obj->p_mTransform->mScale));
					}
					else
					{
						ImGui::Text("Position");
						ImGui::SliderFloat("x", &obj->p_mTransform->mPosition.x, obj->p_mTransform->mPosition.x -0.3f, obj->p_mTransform->mPosition.x + 0.3f);
						ImGui::SliderFloat("y", &obj->p_mTransform->mPosition.y, obj->p_mTransform->mPosition.y -0.3f, obj->p_mTransform->mPosition.y + 0.3f);
						ImGui::SliderFloat("z", &obj->p_mTransform->mPosition.z, obj->p_mTransform->mPosition.z -0.3f, obj->p_mTransform->mPosition.z + 0.3f);
						ImGui::SliderFloat3(Rot.c_str(), (float*)&(obj->p_mTransform->mRotation), -RotationMax, RotationMax);
						ImGui::SliderFloat3(Sca.c_str(), (float*)&(obj->p_mTransform->mScale), 0.0f, ScaleMax);
					}

					// 削除
					if (ImGui::Button("Delete"))
					{
						if (obj->sObjectName != "Camera")
						{
							obj->bDestroy = true;
						}
					}
					ImGui::TreePop();
				}
				index++;
			}
			ImGui::TreePop();
		}

	}
	ImGui::End();
}

void Scene::SaveLevelData()
{
	// セーブ用ファイルを開く
	FileOperator* File = new FileOperator(CreateLevelFileName().c_str());
	File->Write("exp,ModelName,ObjectName,Posx,Posy,Posz,Rotx,Roty,Rotz,Scax,Scay,Scaz", false);
	File->Write("type,char,char,float,float,float,float,float,float,float,float,float");
	for (auto List : mSceneObjectList)
	{
		for (auto Obj : List)
		{
			// 現段階でメッシュ持ちモデルを保存する
			if (Obj->GetComponent<Com_StaticMesh>() != nullptr)
			{
				File->Write("data,");
				File->Write(Obj->GetComponent<Com_StaticMesh>()->GetModelName(), false);
				File->Write(",", false);
				File->Write(Obj->sObjectName, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.z, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.z, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.z, false);
			}
			/*if (Obj->GetComponent<Com_SkeletalMesh>() != nullptr)
			{
				File->Write(Obj->GetComponent<Com_SkeletalMesh>()->GetModelName());
				File->Write(",", false);
				File->Write(Obj->sObjectName, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mPosition.z, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mRotation.z, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.x, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.y, false);
				File->Write(",", false);
				File->Write(Obj->p_mTransform->mScale.z, false);
			}*/
		}
	}
	File->Close();
	delete File;
}

void Scene::ReadLevelData()
{
	// 設定されたオブジェクトの読み込み処理
	vector<string> ObjectsStr = ReadDataCSVAll(CreateLevelFileName().c_str());

	for (auto vec : ObjectsStr)
	{
		vector<string> datastr = SeparateString(vec, ',');
		if (datastr[0] == "data")
		{
			// オブジェクト名で読み込み
			GameObject* Object = new GameObject(datastr[2]);
			Com_StaticMesh* Mesh_Buf = new Com_StaticMesh();
			// 使用するシェーダーの設定
			Mesh_Buf->SetVertex("VS_GBufferPBR");
			Mesh_Buf->SetPixel("PS_GBufferPBR");
			Mesh_Buf->SetModelData(datastr[1].c_str());
			Object->AddComponent(Mesh_Buf);
			Object->SetPosition(stof(datastr[3]), stof(datastr[4]), stof(datastr[5]));
			Object->SetRotation(stof(datastr[6]), stof(datastr[7]), stof(datastr[8]));
			Object->SetScale(stof(datastr[9]), stof(datastr[10]), stof(datastr[11]));
			AddGameObject(Object, 1);
		}
	}
}

void Scene::ReadLightData()
{
	// ライトの読み込み処理
	vector<string> AllData = ReadDataCSVAll(CreateLightFileName().c_str());
	for (int i = 0; i < AllData.size(); i++)
	{
		vector<string> LightData = SeparateString(AllData[i], ',');
		if (LightData[0] == "Directional")
		{
			DirectionalLight* light = LightPool::GetDirectioLight();

			light->mPosition = Vector3(stof(LightData[1]), stof(LightData[2]), stof(LightData[3]));
			light->Direction = Vector4(stof(LightData[4]), stof(LightData[5]), stof(LightData[6]), 0.0f);
			light->Diffuse = Vector4(stof(LightData[7]), stof(LightData[8]), stof(LightData[9]), stof(LightData[10]));
			light->Ambient = Vector4(stof(LightData[11]), stof(LightData[12]), stof(LightData[13]), 0.0f);
			light->BaseColor = Vector4(stof(LightData[14]), stof(LightData[15]), stof(LightData[16]), 0.0f);

		}
		else if (LightData[0] == "Point")
		{
			PointLight* light = new PointLight();
			// ポジションと範囲の読み込み
			light->Position = Vector4(stof(LightData[1]), stof(LightData[2]), stof(LightData[3]), stof(LightData[4]));
			// 色と光の強さの読み込み
			light->Diffuse = Vector4(stof(LightData[5]), stof(LightData[6]), stof(LightData[7]), stof(LightData[8]));
			LightPool::Push(light);
		}
		else if (LightData[0] == "Spot")
		{
			SpotLight* light = new SpotLight();
			// ポジション
			light->Position;
			// 色と光の強さ
			light->Diffuse;
			// 影響範囲
			light->range;
			// 向き
			light->Direction;
			// 角度
			light->angle;
			LightPool::Push(light);
		}
	}
}

void Scene::ReadMaterialData(const char* _FileName)
{
	MaterialPool::ReadMaterialData(_FileName);
}


int Scene::LayerCheck(GameObject* _obj)
{
	return 1;
}

std::string Scene::GetSceneName()
{
	return mSceneName;
}
