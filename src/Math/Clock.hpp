#pragma once

#include <chrono>

namespace Engine
{
class Clock
{
public:
	Clock() :
		_lastTime(getTime())
	{}
	~Clock()
	{}

	float getDelta()
	{
		float currentTime = getTime();

		float delta = currentTime - _lastTime;
		_lastTime = currentTime;

		return delta;
	}
	static float getTime()
	{
		std::chrono::duration<float> seconds = std::chrono::high_resolution_clock::now().time_since_epoch();
		return seconds.count();
	}

private:
	float _lastTime;
};
} // namespace Engine
