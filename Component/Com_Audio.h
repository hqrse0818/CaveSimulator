#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include "Component.h"
#pragma comment(lib, "xaudio2.lib")

class Com_Audio :
    public Component
{
private:

    // static
    static IXAudio2* s_p_mXAudio;
    static IXAudio2MasteringVoice* s_p_mMasteringVoice;
    static float s_fMasterVolume;
    // オーディオターゲット
    static GameObject* s_p_mTarget;

    // 各インスタンスの変数
    IXAudio2SourceVoice* p_mSourceVoice{};
    BYTE* p_mSoundData{};
    X3DAUDIO_EMITTER emitter{};
    X3DAUDIO_LISTENER listener{};

    int iLength{};
    int iPlayLength{};

    // 実際に流す音声のボリューム
    float fVolume;
    // 値が大きいほど遠くでも大きい音が聞こえる
    float fSoundScaleRatio = 10.0f;

    bool bBGM;
    bool bUseTarget = false;
    bool bPlayAtLocation = false;
    
public:

    // static
    static void InitMaster();
    static void UninitMaster();
    static void SetTarget(GameObject* target);

    // 各インスタンスの関数
    void Init();
    void Uninit();
    void Update();

    void Load(const char* _FileName);
    void Play(bool Loop = false);
    void Stop();

    void SetSoundRatio(float _val)
    {
        fSoundScaleRatio = _val;
    };

    void SetVolume(float _volume);
    void SetPlayAtLocation(bool _val);
    void SetUseTarget(bool _val)
    {
        bUseTarget = _val;
    };

    static void SetMasterVolume(float _volume);
};

