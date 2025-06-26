#pragma once

extern "C" double glfwGetTime();

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
		return static_cast<float>(glfwGetTime());
	}

private:
	float _lastTime;
};
