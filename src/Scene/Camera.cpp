#include "Camera.hpp"

#include "../Math/Angles.hpp"
#include "../Math/Matrix4.hpp"

#include <cmath>


Camera::Camera(float fov, float aspect, float nearPlane, float farPlane) :
	_fov(fov), _aspectRatio(aspect), _nearClip(nearPlane), _farClip(farPlane)
{
	_target = Vector3(0, 0, 0);
	Vector3 forward = (_target - _position).normalized();
	_upDirection = Vector3(0, 1, 0);

	if (std::abs(forward.y) > 0.99f)
	{
		_upDirection = Vector3(0, 0, (forward.y > 0) ? -1.0f : 1.0f);
	}
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
	{
		worldUp = Vector3(0, 0, 1);
	}

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

Matrix4 Camera::getViewMatrix() const
{
	return Matrix4::WithLookAt(_position, _target, _upDirection);
}

Matrix4 Camera::getProjectionMatrix() const
{
	return Matrix4::WithPerspective(radians(_fov), _aspectRatio, _nearClip, _farClip);
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
