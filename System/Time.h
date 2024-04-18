#pragma once
#include	<thread>
#include	<iostream>
#include	<windows.h>
#include <chrono>

#pragma comment(lib, "Winmm.lib")

// �V���O���g���Ǘ���Time�N���X

class Times
{
private:

	static Times* p_mInstance;
	Times() {};
	~Times() {};

	// �O�񎞊�
	static std::chrono::high_resolution_clock::time_point s_mLastTime;
	static std::chrono::high_resolution_clock::time_point s_mCurrentTime;

	// �o�ߎ���(�~���b)
	static std::chrono::microseconds s_mDeltaMicroTime;
	// �ϊ���f���^�^�C��
	static float s_fDeltaTime;

	// nanosec�o�͗p�J�E���^�[
	static std::chrono::high_resolution_clock::time_point s_tpstart, s_tpend;

	// FPS�v���p
	static int FPSTimer;
	static int iFrameCount;
public:
	// ��ʏ�̃I�u�W�F�N�g�̓����Ȃǂ��~�߂�ꍇ��0�ɂ���
	static float s_fTimeScale;

public:

	static float GetDeltaTime()
	{
		return s_fDeltaTime;
	}

	static Times* GetInstance()
	{
		return p_mInstance;
	}

	static void Init();

	static void Update();

	static void Uninit();

	static void CountStart();
	static float CountStop();
	static long long CountStopll();
};

#define Time Times::GetInstance()