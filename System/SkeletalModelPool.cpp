#include "SkeletalModelPool.h"

std::unordered_map<std::string, SkeletalMeshLoader*> SkeletalModelPool::p_mModelmap{};

SkeletalMeshLoader* SkeletalModelPool::GetLoaderData(const char* _ModelName)
{
	if (p_mModelmap.count(_ModelName) > 0)
	{
		return p_mModelmap[_ModelName];
	}

	return nullptr;
}

SKELETALMESH_DATA* SkeletalModelPool::GetModelData(const char* _ModelName)
{
	if (p_mModelmap.count(_ModelName) > 0)
	{
		return p_mModelmap[_ModelName]->GetModelData();
	}

	return nullptr;
}

void SkeletalModelPool::Uninit()
{
	for (auto data : p_mModelmap)
	{
		data.second->Uninit();
		delete data.second;
	}
	p_mModelmap.clear();
}

void SkeletalModelPool::Push(const char* _Name, SkeletalMeshLoader* _Data)
{
	if (p_mModelmap.count(_Name) > 0)
	{
		return;
	}
	p_mModelmap[_Name] = _Data;
}
