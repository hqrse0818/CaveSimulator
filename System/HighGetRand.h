#pragma once
#include <iostream>
#include <random>
#include <chrono>

// 高精度乱数

class HighRand
{
private:

	// 乱数の種
	static unsigned seed;
	// 乱数ジェネレーター
	static std::default_random_engine generator;
	static HighRand* instance;
	
public:
	static void HighRandInit();
	
	// int用乱数取得関数
	// 最小値以上最大値以下の値を返す
	static int GetRand(const int _min, const int _max);

	// float用乱数取得関数
	// 最小値より大きく、最大値未満の値を返す
	static float fGetRand(const int _min, const int _max, const int period);

	static HighRand* GetInstance();

	static void Uninit();
};

#define HighRandom HighRand::GetInstance()