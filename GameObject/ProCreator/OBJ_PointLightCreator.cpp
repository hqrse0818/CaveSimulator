#include "OBJ_PointLightCreator.h"
#include "../../System/SolutionCSV.h"
#include "../../System/LightPool.h"
#include "../../Utility.h"

using namespace DirectX::SimpleMath;
using namespace std;

OBJ_PointLightCreator::OBJ_PointLightCreator()
{
}

OBJ_PointLightCreator::OBJ_PointLightCreator(const char* _Name)
{
	sObjectName = _Name;
}

OBJ_PointLightCreator::OBJ_PointLightCreator(const char* _Name, const char* _FileName)
	: OBJ_PointLightCreator(_Name)
{
	// ポイントライトの読み込み
	vector<string> Alldata = ReadDataCSVAll(_FileName);
	int DataNum = static_cast<int>(Alldata.size()) - 2;
	int InitDataRow = 2;
	int datastart = 1;
	for (int index = 0; index < DataNum; index++)
	{
		PointLight* pPointLight = new PointLight();
		vector<string> data = SeparateString(Alldata[InitDataRow + index], ',');
		pPointLight->Position = Vector4(stof(data[datastart + 0]), stof(data[datastart + 1]), stof(data[datastart + 2]), stof(data[datastart + 3]));
		pPointLight->Diffuse = Vector4(stof(data[datastart + 4]), stof(data[datastart + 5]), stof(data[datastart + 6]), stof(data[datastart + 7]));
		LightPool::Push(pPointLight);
	}
}

OBJ_CustomPointLightCreator::OBJ_CustomPointLightCreator()
{
}

OBJ_CustomPointLightCreator::OBJ_CustomPointLightCreator(const char* _Name)
{
	sObjectName = _Name;
}

OBJ_CustomPointLightCreator::OBJ_CustomPointLightCreator(const char* _Name, const char* _FileName)
	: OBJ_CustomPointLightCreator(_Name)
{
	// ポイントライトの読み込み
	vector<string> Alldata = ReadDataCSVAll("asset/data/csv/CustomPointLight.csv");
	int DataNum = static_cast<int>(Alldata.size()) - 2;
	int InitDataRow = 2;
	int datastart = 1;
	for (int index = 0; index < DataNum; index++)
	{
		CustomPointLight* pPointLight = new CustomPointLight();
		vector<string> data = SeparateString(Alldata[InitDataRow + index], ',');
		pPointLight->SetColor(stof(data[datastart + 0]), stof(data[datastart + 1]), stof(data[datastart + 2]), stof(data[datastart + 3]));
		pPointLight->mInitRange = stof(data[datastart + 4]);
		pPointLight->mMaxRange = stof(data[datastart + 5]);
		pPointLight->mMinRange = stof(data[datastart + 6]);
		pPointLight->mGrowSpeed = stof(data[datastart + 7]);
		pPointLight->mAngleXZ = stof(data[datastart + 8]);
		pPointLight->mAngleY = stof(data[datastart + 9]);
		pPointLight->mAngleSpeedXZ = stof(data[datastart + 10]);
		pPointLight->mAngleSpeedY = stof(data[datastart + 11]);
		pPointLight->AxisPos.x = stof(data[datastart + 12]);
		pPointLight->AxisPos.y = stof(data[datastart + 13]);
		pPointLight->AxisPos.z = stof(data[datastart + 14]);
		pPointLight->mLength = stof(data[datastart + 15]);
		pPointLight->InitPointLight();
		mLightList.push_back(pPointLight);
		LightPool::Push(pPointLight->GetLight());
	}
}

void OBJ_CustomPointLightCreator::Update()
{
	for (auto lig : mLightList)
	{
		lig->UpdatePointLight();
	}
}

void OBJ_CustomPointLightCreator::Uninit()
{
	GameObject::Uninit();
	for (auto lig : mLightList)
	{
		SAFE_NULL(lig);
	}
}
