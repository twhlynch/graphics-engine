#pragma once

#include "../Math/Quaternion.hpp"
#include "../Math/Vector3.hpp"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float fov, float aspect, float nearPlane, float farPlane);

	void setPosition(const Vector3 &position);
	void setTarget(const Vector3 &target);
	void setAspectRatio(float aspect);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	Vector3 getForward() const;
	Vector3 getRight() const;
	Vector3 getUp() const;

	Vector3 &getPosition();
	Vector3 &getTarget();

private:
	Vector3 _position;
	Vector3 _target;
	Vector3 _upDirection;

	float _fov;
	float _aspectRatio;
	float _nearClip;
	float _farClip;
};
