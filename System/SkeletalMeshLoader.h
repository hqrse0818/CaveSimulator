#pragma once

// ���f���f�[�^�ǂݍ��݃N���X
// Assimp���g�p

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif // DEBUG

#include <unordered_map>
#include <vector>

#include "../DirectX/renderer.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/Importer.hpp"

enum class SKELETALMESH_TYPE
{
    Normal,
    PBR,
    Tangent,
};

struct DEFORM_VERTEX
{
    aiVector3D Position;
    aiVector3D Normal;
    int BoneNum;
    std::string BoneName[4];
    int BoneIndex[4];
    float BoneWeight[4];
};

struct BONE
{
    aiMatrix4x4 Matrix;                     // ���W�ϊ��Ɏg�p����s��
    aiMatrix4x4 AnimationMatrix;    // �P�̃A�j���[�V�����p�s��
    aiMatrix4x4 OffsetMatrix;           // �{�[���I�t�Z�b�g�s��
    aiQuaternion BlendRotFrom;        // ���[�V�����u�����h�̑J�ڑO�A�j���[�V�����̉�]
    aiQuaternion BlendRotTo;            // ���[�V�����u�����h�ڍs��̃A�j���[�V�����̉�]
    aiVector3D BlendPosFrom;       // ���[�V�����u�����h�J�ڑO�A�j���[�V�����̈ʒu
    aiVector3D BlendPosTo;            // ���[�V�����u�����h�J�ڌ�̃A�j���[�V�����̈ʒu
    int mIndex;
};

struct BoneCompMatrix
{
    DirectX::SimpleMath::Matrix BoneCombMtx[400];   // �{�[���R���r�l�[�V�����s��
};

struct AnimationData
{
    const aiScene* pAnimation;
    unsigned int LastKey = 0;
};

struct SKELETALMESH_DATA
{
    // �`��^�C�v
    SKELETALMESH_TYPE mType = SKELETALMESH_TYPE::Normal;
    // Assimp�ǂݍ��ݗp
    const aiScene* p_mAiScene = nullptr;
    // �A�j���[�V�������X�g
    std::unordered_map<std::string, AnimationData>map_mAnimation;
    // ���_�o�b�t�@
    ID3D11Buffer** pp_mVertexBuffer;
    // �C���f�b�N�X�o�b�t�@
    ID3D11Buffer** pp_mIndexBuffer;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> map_mTexture;
    // �}�e���A���f�[�^
    std::vector<MATERIAL> vec_material;
    // �ό`��̒��_�f�[�^
    std::vector < DEFORM_VERTEX >* vec_mDeformVertex;
    // �{�[���f�[�^
    std::unordered_map<std::string, BONE> map_mBone;
};

class SkeletalMeshLoader
{
private:
    SKELETALMESH_DATA* p_mModelData{};
    // �g�p�����}�e���A���̃C���f�b�N�X���i�[
    std::unordered_map<unsigned int, unsigned int> IDInfo;
    int mNumVertices = 0;
private:
    // �{�[�����ċA�I�ɍ쐬
    void CreateBone(aiNode* _node);
    ID3D11ShaderResourceView* LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName);

public:
    SkeletalMeshLoader();

    // ���f���̓ǂݍ���
    void LoadModel(const char* _FileName, bool _bLoadMaterial);
    // PBR���f���ǂݍ���
    void LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial);
    // �A�j���[�V�����̓ǂݍ���
    void LoadAnimation(const char* _FileName, const char* _AnimName);
    // ���f���f�[�^�����f���f�[�^�v�[���ɓo�^
    void Push(const char* _ModelName);
    // �J��
    void Uninit();
    SKELETALMESH_DATA* GetModelData()
    {
        return p_mModelData;
    }
    // �f�B�t���[�Y�Z�b�g
    void SetAlbedoColor(DirectX::SimpleMath::Color _color, int index);

    // �G�~�b�V�����Z�b�g
    void SetEmission(DirectX::SimpleMath::Color _color, int index);

    // �X�y�L�������x�ݒ�
    void SetMetallic(float _val, int index);

    // ���t�l�X�Z�b�g
    void SetRoughness(float _val, int index);

    int GetVertexNum()
    {
        return mNumVertices;
    }
};

