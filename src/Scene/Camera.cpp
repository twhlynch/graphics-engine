#include "Camera.hpp"

#include <cmath>

#include <glm/glm.hpp>

Camera::Camera(float fov, float aspect, float nearPlane, float farPlane) :
	_position(Vector3()), _fov(fov), _aspectRatio(aspect), _nearClip(nearPlane), _farClip(farPlane)
{
	_target = Vector3(0, 0, 0);
	Vector3 forward = (_target - _position).normalized();
	_upDirection = Vector3(0, 1, 0);

	if (std::abs(forward.y) > 0.99f)
		_upDirection = Vector3(0, 0, (forward.y > 0) ? -1.0f : 1.0f);
}

void Camera::setPosition(const Vector3 &position)
{
	_position = position;
}

void Camera::setTarget(const Vector3 &target)
{
	_target = target;

	Vector3 forward = (_target - _position).normalized();
	Vector3 worldUp = Vector3(0, 1, 0);

	if (std::abs(forward.y) > 0.99f)
		worldUp = Vector3(0, 0, 1);

	Vector3 right = forward.cross(worldUp).normalized();
	_upDirection = right.cross(forward).normalized();
}

void Camera::setAspectRatio(float aspect)
{
	_aspectRatio = aspect;
}

Vector3 &Camera::getPosition()
{
	return _position;
}

Vector3 &Camera::getTarget()
{
	return _target;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_position.asGLM(), _target.asGLM(), _upDirection.asGLM());
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(_fov), _aspectRatio, _nearClip, _farClip);
}

Vector3 Camera::getForward() const
{
	return (_target - _position).normalized();
}

Vector3 Camera::getRight() const
{
	return getForward().cross(_upDirection).normalized();
}

Vector3 Camera::getUp() const
{
	return _upDirection;
}