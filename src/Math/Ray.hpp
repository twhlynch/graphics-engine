#pragma once

#include "../Math/Vector3.hpp"
#include "../Rendering/Entity.hpp"
#include <vector>

namespace Engine
{
struct Intersection
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
	void setObjects(std::vector<Entity *> *objects);

	std::vector<Intersection> cast(float length = 100.0f) const;

	Entity *getEntityToVisualize(float length = 100.0f) const;

private:
	void intersect(Entity *entity, std::vector<Intersection> &intersections) const;
	Vector3 *intersectTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c) const;

	Vector3 _position;
	Vector3 _direction;
	std::vector<Entity *> *_objects;
};
} // namespace Engine
