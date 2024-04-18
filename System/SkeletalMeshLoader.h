#pragma once

// モデルデータ読み込みクラス
// Assimpを使用

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
    aiMatrix4x4 Matrix;                     // 座標変換に使用する行列
    aiMatrix4x4 AnimationMatrix;    // 単体アニメーション用行列
    aiMatrix4x4 OffsetMatrix;           // ボーンオフセット行列
    aiQuaternion BlendRotFrom;        // モーションブレンドの遷移前アニメーションの回転
    aiQuaternion BlendRotTo;            // モーションブレンド移行後のアニメーションの回転
    aiVector3D BlendPosFrom;       // モーションブレンド遷移前アニメーションの位置
    aiVector3D BlendPosTo;            // モーションブレンド遷移後のアニメーションの位置
    int mIndex;
};

struct BoneCompMatrix
{
    DirectX::SimpleMath::Matrix BoneCombMtx[400];   // ボーンコンビネーション行列
};

struct AnimationData
{
    const aiScene* pAnimation;
    unsigned int LastKey = 0;
};

struct SKELETALMESH_DATA
{
    // 描画タイプ
    SKELETALMESH_TYPE mType = SKELETALMESH_TYPE::Normal;
    // Assimp読み込み用
    const aiScene* p_mAiScene = nullptr;
    // アニメーションリスト
    std::unordered_map<std::string, AnimationData>map_mAnimation;
    // 頂点バッファ
    ID3D11Buffer** pp_mVertexBuffer;
    // インデックスバッファ
    ID3D11Buffer** pp_mIndexBuffer;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> map_mTexture;
    // マテリアルデータ
    std::vector<MATERIAL> vec_material;
    // 変形後の頂点データ
    std::vector < DEFORM_VERTEX >* vec_mDeformVertex;
    // ボーンデータ
    std::unordered_map<std::string, BONE> map_mBone;
};

class SkeletalMeshLoader
{
private:
    SKELETALMESH_DATA* p_mModelData{};
    // 使用したマテリアルのインデックスを格納
    std::unordered_map<unsigned int, unsigned int> IDInfo;
    int mNumVertices = 0;
private:
    // ボーンを再帰的に作成
    void CreateBone(aiNode* _node);
    ID3D11ShaderResourceView* LoadDiffuseTexture(std::string _FileName, std::string _ModelFileName);

public:
    SkeletalMeshLoader();

    // モデルの読み込み
    void LoadModel(const char* _FileName, bool _bLoadMaterial);
    // PBRモデル読み込み
    void LoadModelPBR(const char* _FileName, const char* _TextName, bool _bLoadMaterial);
    // アニメーションの読み込み
    void LoadAnimation(const char* _FileName, const char* _AnimName);
    // モデルデータをモデルデータプールに登録
    void Push(const char* _ModelName);
    // 開放
    void Uninit();
    SKELETALMESH_DATA* GetModelData()
    {
        return p_mModelData;
    }
    // ディフューズセット
    void SetAlbedoColor(DirectX::SimpleMath::Color _color, int index);

    // エミッションセット
    void SetEmission(DirectX::SimpleMath::Color _color, int index);

    // スペキュラ強度設定
    void SetMetallic(float _val, int index);

    // ラフネスセット
    void SetRoughness(float _val, int index);

    int GetVertexNum()
    {
        return mNumVertices;
    }
};

