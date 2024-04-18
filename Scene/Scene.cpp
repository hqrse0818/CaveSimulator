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

	// �������O�ɌĂԏ���
	PreInit();


	// �J�����̍쐬
	pCamera = new GameObject("Camera");
	pMainCamera = new Com_AngleCamera();
	pCamera->AddComponent(pMainCamera);
	pCamera->InitBase();
	SetMainCamera(static_cast<Com_Camera*>(pMainCamera));
	AddKeyObject(pCamera, 0);

	// �J�����̃^�[�Q�b�g�쐬
	pEmpty = new GameObject("CameraTarget");
	pTransCom = new Com_SimpleTranslate();
	pTransCom->SetCamera(pCamera);
	pEmpty->AddComponent(pTransCom);
	pMainCamera->SetTarget(pEmpty);
	AddKeyObject(pEmpty, 1);


	// �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���r���[�̎擾
	{
		// GBuffer�̎擾
		pGBufferNormal = DirectViewList::GetRTV("RTVNormal");
		pGBufferAlbedo = DirectViewList::GetRTV("RTVAlbedo");
		pGBufferWorld = DirectViewList::GetRTV("RTVWorld");
		pGBufferMaterial = DirectViewList::GetRTV("RTVMaterial");
	}

	// �`��J�n�ʒu�̒�`
	Vector3 InitPos = Vector3(SpriteWidth / 2, SpriteHeight / 2, 0.0f);

	// �f�B�t�@�[�h�����_�����O�p�I�u�W�F�N�g�̍쐬
	{
		// GBuffer
		{
			// �@���`��
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
			// �A���x�h�J���[
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
			// ���[���h���W
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
			// �}�e���A��ID
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
		// �f�B�t�@�[�h�����_�����O�p
		{
			pDeffered = new GameObject("Deffered");
			pDefferedSprite = new Com_Sprite();
			pDefferedSprite->SetVertex("VS_SPRITE");
			pDefferedSprite->SetPixel("PS_DisneyPBR");
			pDeffered->AddComponent(pDefferedSprite);
			// �S���
			pDefferedSprite->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
			pDeffered->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			pDeffered->InitBase();
		}
		// �f�B�t�@�[�h�����_�����O�̌��ʊi�[�p
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

	// ���}�b�v�p�I�u�W�F�N�g
	// ����
	p_mFrontMap = new DynamicEnvironmentMap("FrontMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mFrontMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mFrontMap->InitObject();
	// �w��
	p_mBackMap = new DynamicEnvironmentMap("BackMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mBackMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mBackMap->InitObject();
	// �E��
	p_mRightMap = new DynamicEnvironmentMap("RightMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mRightMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mRightMap->InitObject();
	// ����
	p_mLeftMap = new DynamicEnvironmentMap("LeftMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mLeftMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mLeftMap->InitObject();
	// ���
	p_mTopMap = new DynamicEnvironmentMap("TopMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mTopMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mTopMap->InitObject();
	// ����
	p_mBottomMap = new DynamicEnvironmentMap("BottomMap", ENVIRONMENT_SPRITE_WIDTH, ENVIRONMENT_SPRITE_HEIGHT);
	p_mBottomMap->InitRender(ENVIRONMENT_RENDERMAP_WIDTH, ENVIRONMENT_RENDERMAP_WIDTH, false);
	p_mBottomMap->InitObject();
	// �L���[�u�}�b�v�p�����_�[�^�[�Q�b�g
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

	// �L���[�u�}�b�v�쐻�p
	pCubeMapGenerator = new CubeMapGenerator();
	pCubeMapGenerator->Init(ENVIRONMENT_CUBEMAP_HEIGHT, ENVIRONMENT_CUBEMAP_HEIGHT);

	// ���g�̏������������Ă�
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
	// GBuffer�p,���C�e�B���O�p�I�u�W�F�N�g�̉��
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

	// ���}�b�v�I�u�W�F�N�g�̉��
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

	// �L���[�u�}�b�v�̃I�u�W�F�N�g�̉��
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

	// �I�u�W�F�N�g���X�g�̐�������
	for (auto& List : mSceneObjectList)
	{
		// ���X�g�̒��g�̐����������
		for (auto& obj : List)
		{
			obj->Uninit();
			delete obj;
		}
		List.clear();
	}

	// ���g�̉���������Ă�
	Uninit();
}

void Scene::UpdateBase()
{
	bFullScreenOld = bFullScreen;

	// �X�V����(List�d�l)
	for (auto& List : mSceneObjectList)
	{
		for (auto& obj : List)
		{
			obj->UpdateBase();
		}
		// �����_���ɂ��j��`�F�b�N
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
		//	�^�[�Q�b�g�̈ړ�
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
		// �J�����̉�]
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
	

	// ���g�̍X�V�������Ă�
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

	// �X�N���[���T�C�Y�̒���
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

	// �萔�o�b�t�@�X�V
	Renderer::SetCameraPos(pMainCamera->p_mObject->GetPosition());
	LightPool::UpdateDirectionalLight();
	LightPool::UpdateSpotLights();
	LightPool::UpdatePointLights();

}

void Scene::DrawBase()
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	// �`����GBuffer�ɐ؂�ւ���
	// GBuffer�p�����_�[�^�[�Q�b�g
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

	// �`���ύX
	Renderer::SetRenderTargets(4, GBuffer, DSV);
	// �[�x�e�X�g
	Renderer::SetDepthTest(DEPTH_ENABLE_WRITE_TEST);

	// �x�[�X�ƂȂ郏�[���h�ϊ��s��
	DirectX::SimpleMath::Matrix matrix;
	matrix = DirectX::SimpleMath::Matrix::Identity;

	// GBuffer�p�X�̎��s
	// �J�����̕`��(WVP�ϊ��s��̒萔�o�b�t�@�Z�b�g)
	for (auto& obj : mSceneObjectList[ObjListIndex_Camera])
	{
		obj->DrawBase(matrix);
	}
	// �ÓI�I�u�W�F�N�g�̕`��
	for (auto& obj : mSceneObjectList[ObjListIndex_Static])
	{
		obj->DrawBase(matrix);
	}
	// ���I�I�u�W�F�N�g�̕`��
	for (auto& obj : mSceneObjectList[ObjListIndex_Skeletal])
	{
		obj->DrawBase(matrix);
	}

	// �V���h�E�p�X


	// ���}�b�v�p�X
	// ���I���}�b�v�̐���
	//CreateEnvironmentMap();
	// �m�F�p�L���[�u�}�b�v�̐���
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

	// �f�B�t�@�[�h�p�X�̎��s
	RenderTarget* DefferdRTV = DirectViewList::GetRTV("RTVDeffered");
	DefferdRTV->Clear(ClearColor);
	Renderer::SetRenderTargets(1, &DefferdRTV, nullptr);

	// GBuffer���e�N�X�`���Ƃ��ăZ�b�g
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
	

	// �����_�[�^�[�Q�b�g���f�B�t�@�[�h�p�ɐݒ�
	Renderer::SetRenderTargets(1, &DefferdRTV, DSV);

	// �������I�u�W�F�N�g�̕`��
	for (auto& obj : mSceneObjectList[ObjListIndex_Transparent])
	{
		obj->DrawBase(matrix);
	}
	// �G�t�F�N�g�̕`��
	for (auto& obj : mSceneObjectList[ObjListIndex_Effect])
	{
		obj->DrawBase(matrix);
	}

	// 3D�`���̃V�[���̏���
	Draw3D();

	// �����_�[�^�[�Q�b�g���o�b�N�o�b�t�@�Ɏw��
	RenderTarget* BackBuffer = DirectViewList::GetRTV("BackBuffer");
	Renderer::SetRenderTargets(1, &BackBuffer, nullptr);

	switch (mDrawType)
	{
	case Scene::SceneDrawType::GBuffer:
		// GBuffer�̕`��
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
	
	// UI��`��
	for (auto& obj : mSceneObjectList[ObjListIndex_UI])
	{
		obj->DrawBase(matrix);
	}

	// 2D�`���̏���
	Draw2D();
}


void Scene::CreateEnvironmentMap()
{
	// �x�[�X�ƂȂ郏�[���h�ϊ��s��
	DirectX::SimpleMath::Matrix matrix;
	matrix = DirectX::SimpleMath::Matrix::Identity;

	// ���C���J�����̃|�W�V�������擾
	Vector3 CenterPosition = pMainCamera->p_mObject->GetPosition();

	Vector3 Up = pMainCamera->GetUp();
	// ���}�b�v�������̒����_
	Vector3 TargetPosition;
	// ���}�b�v�p�v���W�F�N�V�����ϊ��s��
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

	// 6�t���[����1��e�����̊��}�b�v���X�V�����
	switch (mEnvCurrent)
	{
	case Scene::EnvironmentCurrentRenderType::Front:
	{
		// ���ʕ����̊��}�b�v����
		TargetPosition = CenterPosition;
		TargetPosition.z += 1.0f;
		// ���ʕ����̃r���[�}�g���b�N�X�̐���
		Matrix FrontView = DirectX::XMMatrixLookAtLH(CenterPosition, TargetPosition, Up);
		// �r���[�}�g���b�N�X�̒萔�o�b�t�@���Z�b�g
		Renderer::SetViewMatrix(&FrontView);
		// �f�B�t�@�[�h�����_�����O�Ŏg�p����GBuffer�������_�[�^�[�Q�b�g�ɐݒ肷��
		p_mFrontMap->SetGBuffer();
		// ���}�b�v�ɕ`�悷��I�u�W�F�N�g�̕`��
		RenderEnvironmentObject(matrix);
		// �f�B�t�@�[�h�����_�����O�����s���ĕ`�挋�ʂ𐳖ʗp�̊��}�b�v�Ɋi�[
		p_mFrontMap->ExcuteRendering(pCubeMapGenerator->GetCubeMap());
	}
		break;
	case Scene::EnvironmentCurrentRenderType::Back:
	{
		// �w�ʕ����̊��}�b�v����
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
		// �E�ʕ����̊��}�b�v����
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
		// ���ʕ����̊��}�b�v����
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
		// ��ʕ����̊��}�b�v����
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
		// ���ʕ����̊��}�b�v����
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
	// �ÓI�I�u�W�F�N�g�̕`��
	for (auto& obj : mSceneObjectList[ObjListIndex_Static])
	{
		obj->DrawBase(_Matrix);
	}
	// ���I�I�u�W�F�N�g�̕`��
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
	// ����
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->Update();
	pCubeMapSprite->SetTexture(p_mLeftMap->GetDefferedTexture());
	pCubeMapObj->Draw();
	// ����
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 3), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mFrontMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// �E��
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 5), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mRightMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// �w��
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 7), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/2));
	pCubeMapSprite->SetTexture(p_mBackMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// ���
	pCubeMapObj->SetPosition(static_cast<float>(ENVIRONMENT_CUBEMAP_WIDTH / 8 * 3), static_cast<float>(ENVIRONMENT_CUBEMAP_HEIGHT/6));
	pCubeMapSprite->SetTexture(p_mTopMap->GetDefferedTexture());
	pCubeMapSprite->Update();
	pCubeMapObj->Draw();
	// ����
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
	// ImGui�ɂ��V�[���̐؂�ւ�
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

	// �V�[�����̊T�v�f�[�^�`��
	if (bShowSceneData)
	{
		ShowGuiSceneData();
	}
	// �V�[�����̃I�u�W�F�N�g�f�[�^�`��
	if (bShowObjectData)
	{
		ShowGuiObjectData();
	}
	// ���f���f�[�^���X�g�̕\��
	if (bShowModelData)
	{
		StaticModelPool::ShowStaticModelGui();
	}
	// �}�e���A���f�[�^���X�g�̕\��
	if (bShowMaterialData)
	{
		MaterialPool::ShowMaterialGui();
	}
	// ���C�g�̃f�[�^���X�g�̕\��
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
	// �V�[�����̃Q�[���I�u�W�F�N�g�̐�
	ImGui::Text("ObjectNum : %d", objNum);
	int staticmodelnum = Com_StaticMesh::GetModelNum();
	int skeletalmodelnum = Com_SkeletalMesh::GetModelNum();
	int modelnum = staticmodelnum + skeletalmodelnum;
	// ���f���̐�
	ImGui::Text("ModelNum : %d", modelnum);
	int staticvertices = Com_StaticMesh::GetVertexNum();
	int skeletalvertices = Com_SkeletalMesh::GetVertexNum();
	int totalvertices = staticvertices + skeletalvertices;
	// ���f���̍��v���_��
	ImGui::Text("TotalVertices : %d", totalvertices);

	ImGui::End();
}

void Scene::ShowGuiObjectData()
{
	ImGui::SetNextWindowPos(ImVec2(MAX_SPRITE_WIDTH, 200.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(SCREEN_WIDTH - MAX_SPRITE_WIDTH), 300.0f), ImGuiCond_Once);
	ImGui::Begin("SceneData");

	// ���x���̃Z�[�u
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
	// �V�[�����̃I�u�W�F�N�g�f�[�^�̕\��(Transform�▼�O�̂�)
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

					// �폜
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
	// �Z�[�u�p�t�@�C�����J��
	FileOperator* File = new FileOperator(CreateLevelFileName().c_str());
	File->Write("exp,ModelName,ObjectName,Posx,Posy,Posz,Rotx,Roty,Rotz,Scax,Scay,Scaz", false);
	File->Write("type,char,char,float,float,float,float,float,float,float,float,float");
	for (auto List : mSceneObjectList)
	{
		for (auto Obj : List)
		{
			// ���i�K�Ń��b�V���������f����ۑ�����
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
	// �ݒ肳�ꂽ�I�u�W�F�N�g�̓ǂݍ��ݏ���
	vector<string> ObjectsStr = ReadDataCSVAll(CreateLevelFileName().c_str());

	for (auto vec : ObjectsStr)
	{
		vector<string> datastr = SeparateString(vec, ',');
		if (datastr[0] == "data")
		{
			// �I�u�W�F�N�g���œǂݍ���
			GameObject* Object = new GameObject(datastr[2]);
			Com_StaticMesh* Mesh_Buf = new Com_StaticMesh();
			// �g�p����V�F�[�_�[�̐ݒ�
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
	// ���C�g�̓ǂݍ��ݏ���
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
			// �|�W�V�����Ɣ͈͂̓ǂݍ���
			light->Position = Vector4(stof(LightData[1]), stof(LightData[2]), stof(LightData[3]), stof(LightData[4]));
			// �F�ƌ��̋����̓ǂݍ���
			light->Diffuse = Vector4(stof(LightData[5]), stof(LightData[6]), stof(LightData[7]), stof(LightData[8]));
			LightPool::Push(light);
		}
		else if (LightData[0] == "Spot")
		{
			SpotLight* light = new SpotLight();
			// �|�W�V����
			light->Position;
			// �F�ƌ��̋���
			light->Diffuse;
			// �e���͈�
			light->range;
			// ����
			light->Direction;
			// �p�x
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
