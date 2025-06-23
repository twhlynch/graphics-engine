#pragma once

#include <random>

class Random
{
public:
	static void seed(unsigned int s)
	{
		srand(s);
	}

	template <typename T>
	static T range(T min, T max)
	{
		return min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (max - min)));
	}
};
