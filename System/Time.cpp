#include "Time.h"
#include <winapifamily.h>
#include	<timeapi.h>
#include <iostream>

using namespace std;

Times* Times::p_mInstance = nullptr;

float Times::s_fDeltaTime = 0;
float Times::s_fTimeScale = 1.0f;
chrono::high_resolution_clock::time_point Times::s_mLastTime;
chrono::high_resolution_clock::time_point Times::s_mCurrentTime;
chrono::microseconds Times::s_mDeltaMicroTime;
int Times::FPSTimer = 1000;
int Times::iFrameCount = 0;
chrono::high_resolution_clock::time_point Times::s_tpstart{};
chrono::high_resolution_clock::time_point Times::s_tpend{};

void Times::Init()
{
	if (p_mInstance == nullptr)
	{
		p_mInstance = new Times;

		// バグをなくすためのデルタタイムを初期化
		//timeBeginPeriod(1); // 精度を１ミリ秒に上げる
		s_mCurrentTime = chrono::high_resolution_clock::now();
		s_mLastTime = s_mCurrentTime;
		s_mDeltaMicroTime = chrono::duration_cast<chrono::microseconds>(s_mCurrentTime - s_mLastTime);

		//s_fDeltaTime /= 1000.0f;
	}
}

void Times::Update()
{
	iFrameCount++;
	timeBeginPeriod(1); // 精度を１ミリ秒に上げる

	s_mCurrentTime = chrono::high_resolution_clock::now();
	s_mDeltaMicroTime = chrono::duration_cast<chrono::microseconds>(s_mCurrentTime - s_mLastTime);
	
	s_fDeltaTime = s_mDeltaMicroTime.count() / 1000000.0f;

	s_mLastTime = s_mCurrentTime;
}

void Times::Uninit()
{
	delete p_mInstance;
}

void Times::CountStart()
{
	s_tpstart = chrono::high_resolution_clock::now();
}

float Times::CountStop()
{
	s_tpend = chrono::high_resolution_clock::now();
	chrono::nanoseconds elapsed_time =
		chrono::duration_cast<chrono::nanoseconds>(s_tpend - s_tpstart);

	long long nano = elapsed_time.count();
	float ns = nano / 1000000000.0f;

	return ns;
}

long long Times::CountStopll()
{
	s_tpend = chrono::high_resolution_clock::now();
	chrono::nanoseconds elapsed_time =
		chrono::duration_cast<chrono::nanoseconds>(s_tpend - s_tpstart);

	long long nano = elapsed_time.count();

	return nano;
}