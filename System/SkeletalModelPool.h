#pragma once
#include "SkeletalMeshLoader.h"

#include <unordered_map>
#include <string>

class SkeletalModelPool
{
private:
	static std::unordered_map<std::string, SkeletalMeshLoader*> p_mModelmap;
	SkeletalModelPool() {};

public:
	static SkeletalMeshLoader* GetLoaderData(const char* _ModelName);

	static SKELETALMESH_DATA* GetModelData(const char* _ModelName);

	static void Uninit();

	static void Push(const char* _Name, SkeletalMeshLoader* _Data);
};

