#pragma once

// ���f���f�[�^�ǂݍ��݃N���X
// Assimp���g�p

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
	// �`��^�C�v
	STATICMESH_TYPE mType = STATICMESH_TYPE::Normal;
	// Assimp�f�[�^
	const aiScene* p_mAiScene = nullptr;
	// ���_�o�b�t�@
	ID3D11Buffer** pp_mVertexBuffer{};
	// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer** pp_mIndexBuffer{};
	// �}�e���A���f�[�^
	std::unordered_map<std::string, ID3D11ShaderResourceView*> map_mTexture;
	std::vector<MATERIAL> vec_material;
};

class StaticMeshLoader
{
private:
	STATICMESH_DATA* p_mModelData{};
	// �g�p�����}�e���A���̃C���f�b�N�X���i�[
	std::unordered_map<unsigned int, unsigned int> IDInfo;

	int mNumVertices;
private:
	ID3D11ShaderResourceView* LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName);

public:
	StaticMeshLoader();

	// ���f���̓ǂݍ���
	void LoadModel(const char* _FileName, bool _bLoadMaterial);
	// PBR���f���ǂݍ���
	void LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial);

	// �^���W�F���g�܂ރ��f���ǂݍ���
	void LoadModelIncludeTanget(const char* _FileName, bool _bLoadMaterial);
	// ���f���f�[�^�����f���v�[���ɓo�^
	void Push(const char* _ModelName);
	// �J��
	void Uninit();

	int GetVertexNum()
	{
		return mNumVertices;
	}

	// �f�[�^�擾
	STATICMESH_DATA* GetModelData()
	{
		return p_mModelData;
	}
};

