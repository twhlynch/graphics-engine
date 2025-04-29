#include "Controller.hpp"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <cmath>
#include <iostream>

Controller::Controller(Camera *camera, Window *window, float speed, float sprintMultiplier, float sensitivity) :
	_camera(camera), _window(window), _speed(speed), _sprintMultiplier(sprintMultiplier), _sensitivity(sensitivity), _firstMouse(true), _lastX(0), _lastY(0), _yaw(-90.0f), _pitch(0.0f)
{
	if (camera)
	{
		Vector3 forward = camera->getForward();
		_pitch = glm::degrees(asin(forward.y));
		_yaw = glm::degrees(atan2(forward.z, forward.x));
	}
}

void Controller::update(float delta)
{
	if (!_camera || !_window)
		return;

	processKeyboard(delta);
	processMouse();
}

void Controller::processKeyboard(float delta)
{
	GLFWwindow *win = _window->getGLFWwindow();

	float currentSpeed = _speed;
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		currentSpeed *= _sprintMultiplier;

	Vector3 direction(0.0f, 0.0f, 0.0f);

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		direction.z -= 1.0f;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		direction.z += 1.0f;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		direction.x -= 1.0f;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		direction.x += 1.0f;
	if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
		direction.y -= 1.0f;
	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS)
		direction.y += 1.0f;

	if (direction.x != 0.0f || direction.y != 0.0f || direction.z != 0.0f)
	{
		direction = direction.normalized();
		Vector3 forward = _camera->getForward();
		Vector3 right = _camera->getRight();
		Vector3 up = _camera->getUp();

		Vector3 move = (forward * -direction.z + right * direction.x + up * direction.y) * (currentSpeed * delta);

		Vector3 &position = _camera->getPosition();
		Vector3 &target = _camera->getTarget();

		position = position + move;
		target = target + move;
	}
}

void Controller::processMouse()
{
	GLFWwindow *win = _window->getGLFWwindow();

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
		return;
	}

	float xoffset = static_cast<float>(xpos - _lastX);
	float yoffset = static_cast<float>(_lastY - ypos);
	_lastX = xpos;
	_lastY = ypos;

	xoffset *= _sensitivity;
	yoffset *= _sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	Vector3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front = front.normalized();

	_camera->getTarget() = _camera->getPosition() + front;
}