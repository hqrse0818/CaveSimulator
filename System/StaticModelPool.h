#pragma once
#include "StaticMeshLoader.h"
#include <thread>
#include <unordered_map>
#include <string>


struct LoadThread
{
	std::thread th;
	bool bWorking;
	std::string ModelName;
	std::string TextureName;
	std::string FileName;
	StaticMeshLoader* pLoader;
};

class StaticModelPool
{
private:
	static std::unordered_map<std::string, StaticMeshLoader*> p_mModelmap;
	static std::list<LoadThread*> ThreadList;

public:

	static STATICMESH_DATA* GetModelData(const char* _ModelName);
	static StaticMeshLoader* GetLoaderData(const char* _Name);

	static void ShowStaticModelGui();

	static void Uninit();

	static bool Push(const char* _Name, StaticMeshLoader* _Data);
};

