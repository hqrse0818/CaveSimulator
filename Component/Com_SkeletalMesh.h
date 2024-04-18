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

    // �V�F�[�_�[�ŃA�j���[�V�����̌v�Z���s�����߂̒萔�o�b�t�@
    ID3D11Buffer* p_mCombBuffer{};

    // �Đ����̃A�j���[�V�����̖��O
    const char* p_cPlayAnimationName = "";
    const char* p_cBlendAnimationName = "";

    // ���݂̃A�j���[�V�����̃t���[��
    int iCurrentAnimFrame = 0;
    int iBlendAnimFrame = 0;

    // ���[�v
    bool bLoop = false;

    // �}�e���A���̎g�p
    bool bUseMaterial = false;

    // �A�j���[�V�����Đ��␳�l
    float fAnimSpeed = 0.001f;
    float fAnimCnt = 0.0f;

    // �u�����h�Đ�
    float fBlendSpeed = 0.001f;
    float fBlendCnt = 0.0f;
    float fBlendRate = 0.0f;
    float fBlendPerFrame = 0.05f;

    // ���Đ����ǂ���
    bool bPlayOrder = true;

    // �Đ����̃A�j���[�V�����̍Ō�̃L�[
    unsigned int iLastKey = 0;

    // ���݂̃A�j���[�V�����̃L�[���Ō�̎�true�ɂ���
    bool bLastAnimKey = false;

public:
    Com_SkeletalMesh();
    void Update();
    void Draw();
    void ShadowDraw()override;
    void Uninit();

    // ���f���f�[�^�̃Z�b�g
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

    // �A�j���[�V���������̈ʒu�ŃZ�b�g
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
        // �{�[���s��̍X�V
        void UpdateBoneMatrix(aiNode* _Node, aiMatrix4x4 _Matrix);
        // �Đ�����A�j���[�V���������o�^����Ă��邩�m�F
        bool CheckRegisterdAnimation(const char* _AnimName);
        // �Đ�����A�j���[�V���������邩�̃`�F�b�N
        bool CheckHasAnimation(const char* _AnimName);

        // �A�j���[�V�����̍X�V
        void UpdateAnimation();
        // �u�����h�̍X�V
        void UpdateBlend();
};

