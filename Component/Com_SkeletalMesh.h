#pragma once
#include "Com_Mesh.h"
#include "../System/SkeletalMeshLoader.h"

constexpr int BoneBufferRegister = 3;


class Com_SkeletalMesh :
    public Com_Mesh
{
private:
    static int mNumModels;
    static int mNumTotalVertices;

public:
    enum class AnimationState
    {
        Stop, Play,Blend
    };

private:
    int mNumVertices = 0;

    AnimationState mState = AnimationState::Stop;

    SKELETALMESH_DATA* mModelData = nullptr;

    // シェーダーでアニメーションの計算を行うための定数バッファ
    ID3D11Buffer* p_mCombBuffer{};

    // 再生中のアニメーションの名前
    const char* p_cPlayAnimationName = "";
    const char* p_cBlendAnimationName = "";

    // 現在のアニメーションのフレーム
    int iCurrentAnimFrame = 0;
    int iBlendAnimFrame = 0;

    // ループ
    bool bLoop = false;

    // マテリアルの使用
    bool bUseMaterial = false;

    // アニメーション再生補正値
    float fAnimSpeed = 0.001f;
    float fAnimCnt = 0.0f;

    // ブレンド再生
    float fBlendSpeed = 0.001f;
    float fBlendCnt = 0.0f;
    float fBlendRate = 0.0f;
    float fBlendPerFrame = 0.05f;

    // 順再生かどうか
    bool bPlayOrder = true;

    // 再生中のアニメーションの最後のキー
    unsigned int iLastKey = 0;

    // 現在のアニメーションのキーが最後の時trueにする
    bool bLastAnimKey = false;

public:
    Com_SkeletalMesh();
    void Update();
    void Draw();
    void ShadowDraw()override;
    void Uninit();

    // モデルデータのセット
    bool SetModelData(const char* _ModelName);

    void SetUseMaterial(bool _b)
    {
        bUseMaterial = _b;
    }

    std::string GetCurrentAnimName()
    {
        std::string ret = p_cPlayAnimationName;
        return ret;
    }

    void PlayAnimation(const char* _name, bool _loop = false);
    void BlendAnimation(const char* _name, bool _loop = false);
    void SetBlendRate(float _val);

    void SetPlayAnimation(bool _val);

    void UpdateCurrentAnimFrame();
    void UpdateBlendAnimFrame();

    void SetAnimSpeed(float _val)
    {
        fAnimSpeed = _val;
        if (_val < 0.0f)
        {
            bPlayOrder = false;
        }
        else
        {
            bPlayOrder = true;
        }
    }

    bool GetIsLastKey()
    {
        return bLastAnimKey;
    }

    // アニメーションを特定の位置でセット
    void SetCurrentKeyFrame(int _val)
    {
        iCurrentAnimFrame = _val;
    }

    AnimationState GetState()
    {
        return mState;
    }

    static int GetModelNum()
    {
        return mNumModels;
    }

    static int GetVertexNum()
    {
        return mNumTotalVertices;
    }

private:
        // ボーン行列の更新
        void UpdateBoneMatrix(aiNode* _Node, aiMatrix4x4 _Matrix);
        // 再生するアニメーション名が登録されているか確認
        bool CheckRegisterdAnimation(const char* _AnimName);
        // 再生するアニメーションがあるかのチェック
        bool CheckHasAnimation(const char* _AnimName);

        // アニメーションの更新
        void UpdateAnimation();
        // ブレンドの更新
        void UpdateBlend();
};

