#include "OBJ_Creator.h"
#include "../ComponentHeader.h"
#include "../System/SolutionCSV.h"

using namespace std;
using namespace DirectX::SimpleMath;


OBJ_Creator::OBJ_Creator()
{
}

OBJ_Creator::OBJ_Creator(const char* _FileName, const char* _ModelName)
	: OBJ_Creator()
{
	sObjectName = _ModelName;
	sObjectName += "Manager";
	string ss = ReadDataFromCSV(_FileName, 1);
	std::vector<string> snum = SeparateString(ss, ',');
	int CreateNum = stoi(snum[0]);

	for (int i = 0; i < CreateNum; i++)
	{
		string name = "Cobj" + to_string(i);

		GameObject* pObj = new GameObject(name.c_str());
		Com_StaticMesh* pMesh = new Com_StaticMesh();
		pMesh->SetModelData(_ModelName);
		pMesh->SetVertex("VS_GBuffer");
		//pMesh->SetVertex("VS_ScaleModel");
		pMesh->SetPixel("PS_GBuffer");
		//pMesh->SetPixel("PS_Model");
		pObj->AddComponent(pMesh);
		string states = ReadDataFromCSV(_FileName, StartLine + i);
		std::vector<string>sv = SeparateString(states, ',');
		// 文字列分解とステータス設定
		pObj->SetPosition(stof(sv[0]), stof(sv[1]), stof(sv[2]));
		pObj->SetRotation(
			static_cast<float>(Euler_To_Radian(stof(sv[3]))),
			static_cast<float>(Euler_To_Radian(stof(sv[4]))),
			static_cast<float>(Euler_To_Radian(stof(sv[5]))));
		pMesh->UpdateScale(stof(sv[6]), stof(sv[7]), stof(sv[8]));
		mObjList.emplace_back(pObj);
	}
}

OBJ_Creator::OBJ_Creator(const char* _FileName, const char* _ModelName, bool _usematerial)
{
	sObjectName = _ModelName;
	sObjectName += "Manager";
	string ss = ReadDataFromCSV(_FileName, 1);
	std::vector<string> snum = SeparateString(ss, ',');
	int CreateNum = stoi(snum[0]);

	for (int i = 0; i < CreateNum; i++)
	{
		string name = "Cobj" + to_string(i);

		GameObject* pObj = new GameObject(name.c_str());
		Com_StaticMesh* pMesh = new Com_StaticMesh();
		pMesh->SetModelData(_ModelName);
		pMesh->SetVertex("VS_GBuffer");
		//pMesh->SetVertex("VS_ScaleModel");
		pMesh->SetPixel("PS_GBuffer");
		//pMesh->SetPixel("PS_Model");
		pMesh->SetUseMaterial(_usematerial);
		pObj->AddComponent(pMesh);
		string states = ReadDataFromCSV(_FileName, StartLine + i);
		std::vector<string>sv = SeparateString(states, ',');
		// 文字列分解とステータス設定
		pObj->SetPosition(stof(sv[0]), stof(sv[1]), stof(sv[2]));
		pObj->SetRotation(
			static_cast<float>(Euler_To_Radian(stof(sv[3]))),
			static_cast<float>(Euler_To_Radian(stof(sv[4]))),
			static_cast<float>(Euler_To_Radian(stof(sv[5]))));
		pMesh->UpdateScale(stof(sv[6]), stof(sv[7]), stof(sv[8]));
		mObjList.emplace_back(pObj);
	}
}

OBJ_Creator::OBJ_Creator(const char* _FileName, const char* _ModelName, const char* _VSName, const char* _PSName)
	: OBJ_Creator()
{
	sObjectName = _ModelName;
	sObjectName += "Manager";
	string ss = ReadDataFromCSV(_FileName, 1);
	std::vector<string> snum = SeparateString(ss, ',');
	int CreateNum = stoi(snum[0]);

	for (int i = 0; i < CreateNum; i++)
	{
		GameObject* pObj = new GameObject("CObj");
		Com_StaticMesh* pMesh = new Com_StaticMesh();
		pMesh->SetModelData(_ModelName);
		pMesh->SetVertex(_VSName);
		pMesh->SetPixel(_PSName);
		pObj->AddComponent(pMesh);
		string states = ReadDataFromCSV(_FileName, StartLine + i);
		std::vector<string>sv = SeparateString(states, ',');
		// 文字列分解とステータス設定
		pObj->SetPosition(stof(sv[0]), stof(sv[1]), stof(sv[2]));
		pObj->SetRotation(static_cast<float>(Euler_To_Radian(stof(sv[3]))),
			static_cast<float>(Euler_To_Radian(stof(sv[4]))),
			static_cast<float>(Euler_To_Radian(stof(sv[5]))));
		pMesh->UpdateScale(stof(sv[6]), stof(sv[7]), stof(sv[8]));
		mObjList.emplace_back(pObj);
	}
}

void OBJ_Creator::Init()
{
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->InitBase();
		}
	}
}

void OBJ_Creator::Start()
{
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->StartBase();
		}
	}
}

void OBJ_Creator::Update()
{
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->UpdateBase();
		}
	}
}

void OBJ_Creator::Draw()
{
	Matrix mat;
	mat = Matrix::Identity;
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->DrawBase(mat);
		}
	}
}

void OBJ_Creator::ShadowDraw()
{
	Matrix mat;
	mat = Matrix::Identity;
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->ShadowDrawBase(mat);
		}
	}
}

void OBJ_Creator::Uninit()
{
	for (auto obj : mObjList)
	{
		if (obj)
		{
			obj->Uninit();
			delete obj;
		}
	}
	mObjList.clear();
	GameObject::Uninit();
}