#pragma once

#include "../Math/Vector3.hpp"
#include "../Rendering/Entity.hpp"

#include <memory>
#include <vector>

class Intersection
{
public:
	Vector3 point;
	Entity *object;
};

class Ray
{
public:
	Ray();
	Ray(const Vector3 &position, const Vector3 &direction);

	void set(const Vector3 &position, const Vector3 &direction);
	void setObjects(const std::vector<Entity *> &objects);

	std::vector<Intersection> cast(const float length = 100.0f) const;

	Entity *getEntityToVisualize(const float length = 100.0f) const;

private:
	Vector3 _position;
	Vector3 _direction;
	std::vector<Entity *> _objects;
};