#include "HighGetRand.h"

HighRand* HighRand::instance = nullptr;
std::default_random_engine HighRand::generator = {};
unsigned HighRand::seed = 0;

void HighRand::HighRandInit()
{
	if (instance != nullptr)
	{
		delete instance;
	}

	instance = new HighRand();

	// ‚¸“x‚È—”‚Ìí‚Ì¶¬
	seed = std::chrono::system_clock::now().time_since_epoch().count();

	// —”¶¬Ší‚Ì‰Šú‰»
	generator = std::default_random_engine(seed);
}

int HighRand::GetRand(const int _min, const int _max)
{
	// —”‚Ì”ÍˆÍ
	std::uniform_int_distribution<int> Random(_min, _max);

	// —”‚Ì¶¬
	int returnValue = Random(generator);

	return returnValue;
}

float HighRand::fGetRand(const int _min, const int _max, const int period)
{
	int min = _min + 1;
	int max = _max - 1;
	// —”‚Ì”ÍˆÍ
	std::uniform_int_distribution<int> Random(min, max);

	// —”‚Ì¶¬
	int MakeInt = Random(generator);

	Random = std::uniform_int_distribution<int>(0, 9);
	float ten = 0.1f;

	float ret = MakeInt;
	for (int i = 0; i < period; i++)
	{
		int in = Random(generator);
		ret += in * ten;
		ten = ten * 0.1f;
	}

	return ret;
}

HighRand* HighRand::GetInstance()
{
	if (instance == nullptr)
	{
		HighRandInit();
	}
	return instance;
}

void HighRand::Uninit()
{
	if (instance != nullptr)
	{
		delete instance;
	}
}
