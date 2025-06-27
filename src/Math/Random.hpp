#pragma once

#include <chrono>
#include <random>

namespace Engine
{
template <typename T>
static T randRange(T min, T max)
{
	std::mt19937 engine(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(engine);
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(engine);
	}
	return T {};
}
} // namespace Engine
