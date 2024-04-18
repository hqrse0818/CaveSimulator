#pragma once
#include "GameObject.h"
#include "../Component/Com_Shader.h"
#include "../Component/Com_SkeletalMesh.h"
#include "../Component/Com_PlayerMove.h"
#include "../Component/Com_AngleCamera.h"
#include "../Component/Com_BoxCollider.h"

class OBJ_Player :
    public GameObject
{
public:
    enum class PlayerState
    {
        Idle, Run,
    };

private:
    PlayerState mState = PlayerState::Idle;

    bool bInit = true;

    Com_Shader* p_mShaderCom = nullptr;
    Com_SkeletalMesh* p_mModelCom = nullptr;
    Com_PlayerMove* p_mMoveCom = nullptr;
    Com_BoxCollider* p_mColliderCom = nullptr;

    // 外部からセットする
    Com_AngleCamera* p_mCameraCom = nullptr;

    float fIdleCnt = 0.0f;
    float fIdleDuration = 0.01f;

public:
    OBJ_Player();
    OBJ_Player(const char* _name);

    Com_PlayerMove* GetMoveCom()
    {
        return p_mMoveCom;
    }

    Com_SkeletalMesh* GetModelCom()
    {
        return p_mModelCom;
    }

    // カメラのセット
    void SetCamera(Com_AngleCamera* _com)
    {
        p_mCameraCom = _com;
    }

    void Update()override;
};

