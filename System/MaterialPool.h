#pragma once
#include "../Format/ConstantBuffers.h"
#include <string>
#include <d3d11.h>

struct Materials
{
	PBR_MATERIAL Materials[1200];
};

class MaterialPool
{
private:
	static std::vector<PBR_MATERIAL*> MaterialList;
	static std::vector<std::string> MeshNameList;
	static ID3D11Buffer* p_mMaterialBuffer;

public:
	static HRESULT CreateMaterialBuffer();
	static void UpdateMaterials();
	static void SetMaterialBuffer(int _slot);
	static void Push(PBR_MATERIAL* _mat);
	static void PushName(std::string _Name);
	static void Uninit();
	static void Init();
	static void ShowMaterialGui();
	static void SaveMaterialData(const char* _FileName);
	static void ReadMaterialData(const char* _FileName);
	static PBR_MATERIAL* GetMaterial(int _index);
	static std::string GetMeshName(int _index);
	static int GetMaterialSize();
};

