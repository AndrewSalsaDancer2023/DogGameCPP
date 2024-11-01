#include "Utils.h"

namespace utils
{
int GetRandomIntNumber(int minValue, int maxValue)
{
	std::mt19937 engine;
	std::random_device device;
	engine.seed(device());
	std::uniform_int_distribution<int> distribution(minValue, maxValue);
	return distribution(engine);
}

float GetRandomFloatNumber(float minValue, float maxValue)
{
	std::random_device device;
	std::mt19937 engine(device());
		
	std::uniform_real_distribution<float> distribution(minValue, maxValue);
	return distribution(engine);
}
}