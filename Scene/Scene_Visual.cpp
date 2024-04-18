#include "Scene_Visual.h"
#include "../main.h"
#include "../Component/Com_SpringLight.h"
#include "../System/LightPool.h"
#include "../System/MaterialPool.h"
#include "../System/imguiManager.h"
#include "Scene_OutDoor.h"
#include "../System/manager.h"

using namespace DirectX::SimpleMath;
using namespace std;


void Scene_Visual::Init()
{
	mSceneName = "SceneVisual";

	// �X�v���C�g�p�ϐ�
	float Half = 0.5f;
	Vector2 TestSpriteSize = Vector2(600.0f, 150.0f);
	// �J�������x
	fAngleSpeed = 10.0f;


	// �e�X�g�p�V�[���f�[�^�ǂݍ���
	ReadLevelData();
	ReadLightData();
	ReadMaterialData("asset/data/Materials/EditMaterialData.csv");

	// �X�v���C�g�`��e�X�g
	GameObject* pSpriteTest = new GameObject("Sprite");
	Com_Sprite* pSpriteBuf = new Com_Sprite();
	pSpriteBuf->SetTexture("asset/texture/TitleMoji.png");
	pSpriteBuf->SetVertex("VS_SPRITE");
	pSpriteBuf->SetPixel("PS_SPRITE");
	pSpriteBuf->SetUpdate(true);
	pSpriteBuf->SetSize(TestSpriteSize.x, TestSpriteSize.y);
	pSpriteTest->AddComponent(pSpriteBuf);
	pSpriteTest->SetPosition(TestSpriteSize.x * Half, TestSpriteSize.y * Half);
	// 2D�`��z��Ɋi�[
	AddGameObject(pSpriteTest, ObjListIndex_UI);

	// �@���}�b�v��K�p�����I�u�W�F�N�g2��
	GetGameObject("GreenCrystal1")->GetComponent<Com_StaticMesh>()->SetNormalMap("asset/texture/CrystalNormal.png");
	GetGameObject("GreenCrystal1")->GetComponent<Com_StaticMesh>()->SetPixel("PS_GBufferPBRNormal");
	GetGameObject("Arch2")->GetComponent<Com_StaticMesh>()->SetNormalMap("asset/texture/ArchNormal.png");
	GetGameObject("Arch2")->GetComponent<Com_StaticMesh>()->SetPixel("PS_GBufferPBRNormal");
}

void Scene_Visual::Update()
{
	LightPool::UpdatePointLights();
	LightPool::UpdateDirectionalLight();
	MaterialPool::UpdateMaterials();
}

void Scene_Visual::Uninit()
{
	//Com_BillBoard::UninitMain();
}
