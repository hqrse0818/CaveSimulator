#pragma once
#include <iostream>
#include <random>
#include <chrono>

// �����x����

class HighRand
{
private:

	// �����̎�
	static unsigned seed;
	// �����W�F�l���[�^�[
	static std::default_random_engine generator;
	static HighRand* instance;
	
public:
	static void HighRandInit();
	
	// int�p�����擾�֐�
	// �ŏ��l�ȏ�ő�l�ȉ��̒l��Ԃ�
	static int GetRand(const int _min, const int _max);

	// float�p�����擾�֐�
	// �ŏ��l���傫���A�ő�l�����̒l��Ԃ�
	static float fGetRand(const int _min, const int _max, const int period);

	static HighRand* GetInstance();

	static void Uninit();
};

#define HighRandom HighRand::GetInstance()