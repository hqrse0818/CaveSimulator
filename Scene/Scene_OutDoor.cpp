#include "Scene_OutDoor.h"
#include "../System/imguiManager.h"
#include "../System/manager.h"

void Scene_OutDoor::Init()
{
	mSceneName = "SceneOutdoor";

	ReadLevelData();
	ReadLightData();
	ReadMaterialData("asset/data/Materials/EditMaterialData.csv");
}

void Scene_OutDoor::Update()
{
}

void Scene_OutDoor::Uninit()
{
}
