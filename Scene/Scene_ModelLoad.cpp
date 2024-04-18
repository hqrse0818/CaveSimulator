#include "Scene_ModelLoad.h"
#include "../System/SkeletalMeshLoader.h"
#include "../System/StaticMeshLoader.h"
#include "../System/manager.h"
#include "Scene_Title.h"
#include "Scene_PBR.h"
#include "Scene_Visual.h"
#include "Scene_ShaderLoad.h"
#include "../System/MaterialPool.h"
#include "../System/SolutionCSV.h"

using namespace std;

void LoadPlayer()
{
	// プレイヤーモデルの読み込み
	SkeletalMeshLoader* PlayerModel = new SkeletalMeshLoader();
	PlayerModel->LoadModel("asset\\model\\Player\\Player.fbx", true);
	PlayerModel->LoadAnimation("asset\\model\\Player\\Player_Running.fbx", "Run");
	PlayerModel->LoadAnimation("asset\\model\\Player\\Player_Idle.fbx", "Idle");
	PlayerModel->Push("Player");
}

void LoadRocks()
{
	StaticMeshLoader* RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RockA\\RockA.fbx", true);
	RockModel->Push("RockA");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RockB\\RockB.fbx", true);
	RockModel->Push("RockB");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RockC\\RockC.fbx", true);
	RockModel->Push("RockC");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RockD\\RockD.fbx", true);
	RockModel->Push("RockD");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\MedBottom.fbx", true);
	RockModel->Push("MedBottom");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\MedTop.fbx", true);
	RockModel->Push("MedTop");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\ShortBottom.fbx", true);
	RockModel->Push("ShortBottom");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\ShortTop.fbx", true);
	RockModel->Push("ShortTop");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\TallBottom.fbx", true);
	RockModel->Push("TallBottom");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksA\\TallTop.fbx", true);
	RockModel->Push("TallTop");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\BackA\\BackA.fbx", true);
	RockModel->Push("BackA");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\CaveArch\\CaveArch.fbx", true);
	RockModel->Push("Arch");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\RocksB\\StalagmiteA.fbx", true);
	RockModel->Push("StalagmiteA");
	RockModel = new StaticMeshLoader();
	RockModel->LoadModel("asset\\model\\CrystalA\\CrystalMain.fbx", true);
	RockModel->Push("Crystal");

}


void LoadPBRPlayer()
{
	// プレイヤーモデルの読み込み
	SkeletalMeshLoader* PlayerModel = new SkeletalMeshLoader();
	PlayerModel->LoadModelPBR("asset\\model\\Player\\Player.fbx", "Player", true);
	PlayerModel->LoadAnimation("asset\\model\\Player\\Player_Running.fbx", "Run");
	PlayerModel->LoadAnimation("asset\\model\\Player\\Player_Idle.fbx", "Idle");
	PlayerModel->Push("Player");
}

void LoadPBRRoks()
{
	StaticMeshLoader* Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RocksB\\StalagmiteA.fbx", "StalagmiteA", true);
	Loader->Push("PBRStalagmiteA");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\CaveArch\\CaveArch.fbx", "CaveArch",  true);
	Loader->Push("PBRArch");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\CrystalA\\CrystalMain.fbx", "GreenCrystal", true);
	Loader->Push("GreenCrystal");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RockA\\RockA.fbx", "RockA", true);
	Loader->Push("RockA");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RockB\\RockB.fbx", "RockB", true);
	Loader->Push("RockB");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RockC\\RockC.fbx", "RockC", true);
	Loader->Push("RockC");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RockD\\RockD.fbx", "RockD", true);
	Loader->Push("RockD");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RocksA\\ShortBottom.fbx", "ShortBottom", true);
	Loader->Push("ShortBottomRock");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RocksA\\MedBottom.fbx", "MedBottom", true);
	Loader->Push("MediumBottomRock");
	Loader = new StaticMeshLoader();
	Loader->LoadModelPBR("asset\\model\\RocksA\\TallBottom.fbx", "TallBottom", true);
	Loader->Push("TallBottomRock");
}

void LoadCSVPBRModel()
{
	vector<string> datas;
	datas = ReadDataCSVAll("asset/data/csv/LoadModelList.csv");
	for (auto str : datas)
	{
		vector<string> data = SeparateString(str, ',');
		if (data[0] == "data")
		{
			StaticMeshLoader* Loader = new StaticMeshLoader();
			Loader->LoadModelPBR(data[1].c_str(), data[3].c_str(), true);
			Loader->Push(data[2].c_str());
		}
	}
}

void Scene_ModelLoad::Init()
{
	mSceneName = "SceneModelLoad";

	// 初期アセットの読み込み
	LoadPBRRoks();

	// 過去実行中に読み込まれたモデルデータの再読み込み
	LoadCSVPBRModel();
}

void Scene_ModelLoad::Update()
{
	//Manager::SetNextScene<Scene_Title>();
	//Manager::SetNextScene<Scene_PBR>();
	Manager::SetNextScene<Scene_Visual>();
}
