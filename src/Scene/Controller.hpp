#pragma once

#include "../Rendering/Window.hpp"
#include "../Scene/Camera.hpp"
#include "../Scene/Object.hpp"

namespace Engine
{
class Controller : public Object
{
public:
	Controller(Camera *camera, Window *window, float speed = 5.0f, float sprintMultiplier = 5.0f, float sensitivity = 0.2f);

	void update(float delta) override;
	void draw() override {}

private:
	void processKeyboard(float delta);
	void processMouse();

	Camera *_camera;
	Window *_window;

	float _speed;
	float _sprintMultiplier;
	float _sensitivity;

	bool _firstMouse;
	double _lastX, _lastY;
	float _yaw, _pitch;
};
} // namespace Engine
