#pragma once

// モデルデータ読み込みクラス
// Assimpを使用

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif // DEBUG

#include <vector>
#include <unordered_map>

#include "../DirectX/renderer.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/Importer.hpp"

enum class STATICMESH_TYPE
{
	Normal,
	PBR,
	Tangent,
};

struct STATICMESH_DATA
{
	// 描画タイプ
	STATICMESH_TYPE mType = STATICMESH_TYPE::Normal;
	// Assimpデータ
	const aiScene* p_mAiScene = nullptr;
	// 頂点バッファ
	ID3D11Buffer** pp_mVertexBuffer{};
	// インデックスバッファ
	ID3D11Buffer** pp_mIndexBuffer{};
	// マテリアルデータ
	std::unordered_map<std::string, ID3D11ShaderResourceView*> map_mTexture;
	std::vector<MATERIAL> vec_material;
};

class StaticMeshLoader
{
private:
	STATICMESH_DATA* p_mModelData{};
	// 使用したマテリアルのインデックスを格納
	std::unordered_map<unsigned int, unsigned int> IDInfo;

	int mNumVertices;
private:
	ID3D11ShaderResourceView* LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName);

public:
	StaticMeshLoader();

	// モデルの読み込み
	void LoadModel(const char* _FileName, bool _bLoadMaterial);
	// PBRモデル読み込み
	void LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial);

	// タンジェント含むモデル読み込み
	void LoadModelIncludeTanget(const char* _FileName, bool _bLoadMaterial);
	// モデルデータをモデルプールに登録
	void Push(const char* _ModelName);
	// 開放
	void Uninit();

	int GetVertexNum()
	{
		return mNumVertices;
	}

	// データ取得
	STATICMESH_DATA* GetModelData()
	{
		return p_mModelData;
	}
};

