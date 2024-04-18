#pragma once
#include	<thread>
#include	<iostream>
#include	<windows.h>
#include <chrono>

#pragma comment(lib, "Winmm.lib")

// シングルトン管理のTimeクラス

class Times
{
private:

	static Times* p_mInstance;
	Times() {};
	~Times() {};

	// 前回時間
	static std::chrono::high_resolution_clock::time_point s_mLastTime;
	static std::chrono::high_resolution_clock::time_point s_mCurrentTime;

	// 経過時間(ミリ秒)
	static std::chrono::microseconds s_mDeltaMicroTime;
	// 変換後デルタタイム
	static float s_fDeltaTime;

	// nanosec出力用カウンター
	static std::chrono::high_resolution_clock::time_point s_tpstart, s_tpend;

	// FPS計測用
	static int FPSTimer;
	static int iFrameCount;
public:
	// 画面上のオブジェクトの動きなどを止める場合は0にする
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