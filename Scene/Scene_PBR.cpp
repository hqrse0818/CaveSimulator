// �f�B�t�@�[�h�����_�����O�K�p�O��

//#include "Scene_PBR.h"
//#include "../DirectX/DirectViewList.h"
//#include "../GameObject/OBJ_Creator.h"
//#include "../System/Time.h"
//#include "../GameObject/ProCreator/OBJ_PointLightCreator.h"
//#include "../System/LightPool.h"
//#include "../System/MaterialPool.h"
//#include "../System/imguiManager.h"
//#include "../System/StaticModelPool.h"
//#include "../System/FileOperation.h"
//#include "../System/SolutionCSV.h"
//#include "../System/manager.h"
//#include "../Component/Com_Scaling.h"
//
//#define LightPosLen (3)
//#define CameraPosLen (10)
//
//using namespace DirectX::SimpleMath;
//using namespace std;
//
//void Scene_PBR::Init()
//{
//	// �J����
//	GameObject* Camera = new GameObject("Camera");
//	Com_AngleCamera* camera = new Com_AngleCamera();
//	Camera->AddComponent(camera);
//	AddGameObject(Camera, 0);
//	SetMainCamera(camera);
//
//	// �^�C�g�����S
//	pTitleMoji = new GameObject("TitleObj");
//	pTitleSprite = new Com_Sprite();
//	pTitleSprite->SetVertex("VS_SPRITE");
//	pTitleSprite->SetPixel("PS_SPRITE");
//	pTitleSprite->SetSize(400.0f * 1.5f, 100.0f* 1.5f);
//	pTitleSprite->SetTexture("asset/texture/TitleMoji.png");
//	pTitleMoji->AddComponent(pTitleSprite);
//	pTitleMoji->SetPosition(SCREEN_WIDTH - ((400.0f * 1.5f)/2), SCREEN_HEIGHT -((100.0f * 1.5f)/2));
//	pTitleSprite->SetUpdate(true);
//	Com_Scaling* ScaleBuf = new Com_Scaling();
//	pTitleMoji->AddComponent(ScaleBuf);
//	pTitleMoji->InitBase();
//
//	SceneLight = new PointLight();
//	SceneLight->Diffuse = Color(0.0f, 1.0f, 0.0f, 5.0f);
//	SceneLight->Position = Vector4(LightLength, 5.0f, LightLength, 10.0f);
//	LightPool::Push(SceneLight);
//
//	//// �v���C���[
//	//Player = new OBJ_Player("Player");
//	//Player->GetModelCom()->SetVertex("VS_GBufferPBRAnim");
//	//Player->GetModelCom()->SetPixel("PS_GBufferPBR");
//	//// �v���C���[�ɃJ�����̈ʒu����������^����
//	//Player->SetCamera(camera);
//	//AddKeyObject(Player);
//	//// �J�����̃^�[�Q�b�g���v���C���[�ɂ���
//	//camera->SetTarget(Player);
//	//camera->SetDistance(9.0f);
//	//camera->SetHeight(20.0f);
//	//camera->SetAngle(180.0f);
//
//	//Player->GetMoveCom()->SetCameraCom(camera);
//
//	// �ݒ肳�ꂽ���x���f�[�^�̓ǂݍ���(�X�^�e�B�b�N���f���̂�)
//	ReadLevelData("asset/data/csv/Title.csv");
//
//	// �ݒ肳�ꂽ�}�e���A���f�[�^�̓ǂݍ���
//	ReadMaterialData("asset/data/Materials/Material.csv");
//
//
//	// �`�挋�ʏo�͗p�I�u�W�F�N�g�̐ݒ�
//	{
//		// �`��J�n�ʒu�̒�`
//		Vector3 InitPos = Vector3(SpriteWidth / 2, SpriteHeight / 2, 0.0f);
//
//		// �@���`��
//		pNormal = new GameObject("Normal");
//		pNormalSprite = new Com_Sprite();
//		pNormalSprite->SetRelease();
//		pNormalSprite->SetVertex("VS_SPRITE");
//		pNormalSprite->SetPixel("PS_SPRITE");
//		pNormalSprite->SetSize(SpriteWidth, SpriteHeight);
//		pNormalSprite->SetTexture(DirectViewList::GetRTV("RTVNormal"));
//		pNormal->AddComponent(pNormalSprite);
//		pNormal->SetPosition(InitPos.x, InitPos.y);
//		// ���_�o�b�t�@�쐬
//		pNormal->InitBase();
//
//		// �A���x�h�J���[
//		pAlbedo = new GameObject("Albedo");
//		pAlbedoSprite = new Com_Sprite();
//		pAlbedoSprite->SetRelease();
//		pAlbedoSprite->SetVertex("VS_SPRITE");
//		pAlbedoSprite->SetPixel("PS_SPRITE");
//		pAlbedoSprite->SetSize(SpriteWidth, SpriteHeight);
//		pAlbedoSprite->SetTexture(DirectViewList::GetRTV("RTVAlbedo"));
//		pAlbedo->AddComponent(pAlbedoSprite);
//		pAlbedo->SetPosition(InitPos.x * 3, InitPos.y);
//		// ���_�o�b�t�@�쐬
//		pAlbedo->InitBase();
//
//		// ���[���h���W
//		pWorld = new GameObject("World");
//		pWorldSprite = new Com_Sprite();
//		pWorldSprite->SetRelease();
//		pWorldSprite->SetVertex("VS_SPRITE");
//		pWorldSprite->SetPixel("PS_SPRITE");
//		pWorldSprite->SetSize(SpriteWidth, SpriteHeight);
//		pWorldSprite->SetTexture(DirectViewList::GetRTV("RTVWorld"));
//		pWorld->AddComponent(pWorldSprite);
//		pWorld->SetPosition(InitPos.x, InitPos.y * 3);
//		// ���_�o�b�t�@�쐬
//		pWorld->InitBase();
//
//		// �}�e���A��ID
//		pMaterial = new GameObject("World");
//		pMaterialSprite = new Com_Sprite();
//		pMaterialSprite->SetRelease();
//		pMaterialSprite->SetVertex("VS_SPRITE");
//		pMaterialSprite->SetPixel("PS_SPRITE");
//		pMaterialSprite->SetSize(SpriteWidth, SpriteHeight);
//		pMaterialSprite->SetTexture(DirectViewList::GetRTV("RTVMaterial"));
//		pMaterial->AddComponent(pMaterialSprite);
//		pMaterial->SetPosition(InitPos.x * 3, InitPos.y * 3);
//		// ���_�o�b�t�@�쐬
//		pMaterial->InitBase();
//
//		// PBR
//		pPBR = new GameObject("PBR");
//		pPBRSprite = new Com_Sprite();
//		pPBRSprite->SetVertex("VS_SPRITE");
//		pPBRSprite->SetPixel("PS_PBRDef");
//		pPBR->AddComponent(pPBRSprite);
//		// Gui�l��
//		//pPBRSprite->SetSize(MAX_SPRITE_WIDTH, MAX_SPRITE_HEIGHT);
//		//pPBR->SetPosition(InitPos.x * 2, InitPos.y * 2);
//		// �S���
//		pPBRSprite->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
//		pPBR->SetPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
//		// ���_�o�b�t�@�쐬
//		pPBR->InitBase();
//
//	}
//	// �ݒ肳�ꂽ�|�C���g���C�g�̓ǂݍ���
//	OBJ_PointLightCreator* LightCreator = new OBJ_PointLightCreator("Lights", "asset/data/csv/TitleLight.csv");
//	AddGameObject(LightCreator, 3);
//}
//
//void Scene_PBR::Start()
//{
//	Input::SetCursorCenterDisable();
//}
//
//void Scene_PBR::Update()
//{
//	Renderer::SetCameraPos(pMainCamera->p_mObject->GetPosition());
//
//	fTime += Time->GetDeltaTime();
//	float x = cosf(static_cast<float>(Euler_To_Radian(fTime))) * sinf(static_cast<float>(Euler_To_Radian(fTime)));
//	float z = cosf(static_cast<float>(Euler_To_Radian(fTime))) * cosf(static_cast<float>(Euler_To_Radian(fTime)));
//	SceneLight->Position.x = x * LightPosLen;
//	SceneLight->Position.z = z * LightPosLen;
//
//	fCameraAngle += Time->GetDeltaTime() * 0.5f;
//
//	pMainCamera->p_mObject->GetComponent<Com_AngleCamera>()->SetAngle(fCameraAngle);
//
//	if (bRange)
//	{
//		fRange += Time->GetDeltaTime();
//		if (fRange >= fMaxRange)
//		{
//			fRange = fMaxRange;
//			bRange = false;
//		}
//	}
//	else
//	{
//		fRange -= Time->GetDeltaTime();
//		if (fRange <= fMinRange)
//		{
//			fRange = fMinRange;
//			bRange = true;
//		}
//	}
//
//	SceneLight->Position.w = fRange;
//
//	if (bPower)
//	{
//		fPower += Time->GetDeltaTime();
//		if (fPower >= fMaxPower)
//		{
//			fPower = fMaxPower;
//			bPower = false;
//		}
//	}
//	else
//	{
//		fPower -= Time->GetDeltaTime();
//		if (fPower <= fMinPower)
//		{
//			fPower = fMinPower;
//			bPower = true;
//		}
//	}
//
//	SceneLight->Diffuse.w = fPower;
//
//
//	pTitleMoji->UpdateBase();
//
//	// GBuffer�p�����_�[�^�[�Q�b�g
//	RenderTarget* GBuffer[] =
//	{
//		DirectViewList::GetRTV("RTVNormal"),
//		DirectViewList::GetRTV("RTVAlbedo"),
//		DirectViewList::GetRTV("RTVWorld"),
//		DirectViewList::GetRTV("RTVMaterial"),
//	};
//
//	
//
//// �}�e���A���o�b�t�@�̃N���A
//	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
//	GBuffer[3]->Clear(clearColor);
//	DepthStencil* DSV = DirectViewList::GetDSV("DSV");
//
//	// �`���ύX
//	Renderer::SetRenderTargets(4, GBuffer, DSV);
//	// �`��؂�ւ�
//	if ((Controller_Input::GetButton(0, GAMEPAD_RIGHT) == KEYSTATE::KEY_DOWN) &&
//		Controller_Input::GetButton(0, GAMEPAD_SHOULDER_L) == KEYSTATE::KEY_WHILE_UP ||
//		Input::GetKeyState(KEYCODE_L) == KEYSTATE::KEY_WHILE_UP &&
//		Input::GetKeyState(KEYCODE_RIGHT) == KEYSTATE::KEY_DOWN)
//	{
//		int nDraw = static_cast<int>(mDrawWhich) + 1;
//		if (nDraw >= static_cast<int>(DrawType::Max))
//		{
//			nDraw = 0;
//		}
//		mDrawWhich = static_cast<DrawType>(nDraw);
//	}
//	if ((Controller_Input::GetButton(0, GAMEPAD_LEFT) == KEYSTATE::KEY_DOWN) &&
//		Controller_Input::GetButton(0, GAMEPAD_SHOULDER_L) == KEYSTATE::KEY_WHILE_UP ||
//		Input::GetKeyState(KEYCODE_L) == KEYSTATE::KEY_WHILE_UP &&
//		Input::GetKeyState(KEYCODE_LEFT) == KEYSTATE::KEY_DOWN)
//	{
//		int nDraw = static_cast<int>(mDrawWhich) - 1;
//		if (nDraw < static_cast<int>(DrawType::GBuffer))
//		{
//			nDraw = static_cast<int>(DrawType::Max) - 1;
//		}
//	}
//
//	LightPool::UpdatePointLights();
//	LightPool::UpdateDirectionalLight();
//	MaterialPool::UpdateMaterials();
//
//	mDrawWhich = DrawType::Deffered;
//
//	SceneGUIUpdate();
//}
//
//void Scene_PBR::Draw2D()
//{
//	RenderTarget* GBuffer[] =
//	{
//		DirectViewList::GetRTV("RTVNormal"),
//		DirectViewList::GetRTV("RTVAlbedo"),
//		DirectViewList::GetRTV("RTVWorld"),
//		DirectViewList::GetRTV("RTVMaterial"),
//	};
//
//	// �`�����o�b�N�o�b�t�@�Ɏw��
//	RenderTarget* pBack = DirectViewList::GetRTV("BackBuffer");
//	Renderer::SetRenderTargets(1, &pBack, nullptr);
//
//	// �`��Ώۂ̐ؑ�
//	switch (mDrawWhich)
//	{
//	case Scene_PBR::DrawType::GBuffer:
//		pNormal->Draw();
//		pAlbedo->Draw();
//		pWorld->Draw();
//		pMaterial->Draw();
//		break;
//	case Scene_PBR::DrawType::Deffered:
//		pPBRSprite->GetPS()->SetTexture(1, GBuffer[0]);
//		pPBRSprite->GetPS()->SetTexture(2, GBuffer[1]);
//		pPBRSprite->GetPS()->SetTexture(3, GBuffer[2]);
//		pPBRSprite->GetPS()->SetTexture(4, GBuffer[3]);
//		pPBR->Draw();
//		break;
//	case Scene_PBR::DrawType::Max:
//		break;
//	default:
//		break;
//	}
//
//	pTitleMoji->Draw();
//}
//
//void Scene_PBR::Uninit()
//{
//	LightPool::ReleasePointLight();
//	pTitleMoji->Uninit();
//	delete pTitleMoji;
//	pPBR->Uninit();
//	SAFE_NULL(pPBR);
//	pNormal->Uninit();
//	SAFE_NULL(pNormal);
//	pAlbedo->Uninit();
//	SAFE_NULL(pAlbedo);
//	pWorld->Uninit();
//	SAFE_NULL(pWorld);
//	pMaterial->Uninit();
//	SAFE_NULL(pMaterial);
//}
//
//void Scene_PBR::ReadLevelData(const char* _FileName)
//{
//	// �ݒ肳�ꂽ�I�u�W�F�N�g�̓ǂݍ��ݏ���
//	vector<string> ObjectsStr = ReadDataCSVAll(_FileName);
//
//	for (auto vec : ObjectsStr)
//	{
//		vector<string> datastr = SeparateString(vec, ',');
//		if (datastr[0] == "data")
//		{
//			// �I�u�W�F�N�g���œǂݍ���
//			GameObject* Object = new GameObject(datastr[2]);
//			Com_StaticMesh* Mesh_Buf = new Com_StaticMesh();
//			// �d�l�V�F�[�_�[�̐ݒ�
//			Mesh_Buf->SetVertex("VS_GBufferPBR");
//			Mesh_Buf->SetPixel("PS_GBufferPBR");
//			Mesh_Buf->SetModelData(datastr[1].c_str());
//			Object->AddComponent(Mesh_Buf);
//			Object->SetPosition(stof(datastr[3]), stof(datastr[4]), stof(datastr[5]));
//			Object->SetRotation(stof(datastr[6]), stof(datastr[7]), stof(datastr[8]));
//			Object->SetScale(stof(datastr[9]), stof(datastr[10]), stof(datastr[11]));
//			AddGameObject(Object, 2);
//		}
//	}
//}
//
//void Scene_PBR::ReadMaterialData(const char* _FileName)
//{
//	// �ݒ肳�ꂽ�}�e���A���f�[�^�̓ǂݍ���(�C��)
//	vector<string> MaterialStr = ReadDataCSVAll(_FileName);
//
//	// �}�e���A����vector�ŊǗ�����Ă���
//	// �C������}�e���A���̃C���f�b�N�X�f�[�^�Z
//	// or �}�e���A�����ŏC�����邩?
//	unsigned int index = 0;
//
//	for (auto str : MaterialStr)
//	{
//		vector<string> matstr = SeparateString(str, ',');
//		if (matstr[0] == "data")
//		{
//			// �}�e���A���������������`�F�b�N
//			if (matstr[1] == MaterialPool::GetMeshName(index))
//			{
//				PBR_MATERIAL* material = MaterialPool::GetMaterial(index);
//				material->AlbedoColor = Color(stof(matstr[3]), stof(matstr[4]), stof(matstr[5]), stof(matstr[6]));
//				material->Emission = Color(stof(matstr[7]), stof(matstr[8]), stof(matstr[9]), stof(matstr[10]));
//				material->Metalic = stof(matstr[11]);
//				material->Roufhness = stof(matstr[12]);
//				material->MicroFacet = stof(matstr[13]);
//				index++;
//			}
//		}
//	}
//
//	MaterialPool::UpdateMaterials();
//}

//void Scene_PBR::SceneGUIUpdate()
//{
//
//	// �V�[�����̃I�u�W�F�N�g���̕\��
//	int objNum = 0;
//	for (auto list : mSceneObjectList)
//	{
//		objNum += list.size();
//	}
//
//	// ImGui�ɂ��V�[���̐؂�ւ�
//	ImGui::SetNextWindowPos(ImVec2(980.0f, 0.0f), ImGuiCond_Once);
//	ImGui::SetNextWindowSize(ImVec2(520.0f, 120.0f), ImGuiCond_Once);
//	ImGui::Begin("Scene");
//	if (ImGui::Button("GBuffer"))
//	{
//		mDrawWhich = DrawType::GBuffer;
//	}
//	ImGui::SameLine();
//	if (ImGui::Button("Deffered"))
//	{
//		mDrawWhich = DrawType::Deffered;
//	}
//
//
//	ImGui::Checkbox("ShowSceneData", &bShowSceneData);
//	ImGui::Checkbox("ShowObjectData", &bShowObjectData);
//
//	ImGui::SameLine();
//	if (ImGui::Button("ReLoadScene"))
//	{
//		Manager::SetNextScene<Scene_PBR>();
//	}
//
//
//	ImGui::End();
//
//	LightPool::ShowLightGui();
//	MaterialPool::ShowMaterialGui();
//	StaticModelPool::ShowStaticModelGui();
//}
//
//
