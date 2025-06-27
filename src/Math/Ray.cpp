#include "Ray.hpp"
#include "../Rendering/Material.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Shader.hpp"
#include <float.h>

namespace Engine
{
Ray::Ray() {}
Ray::Ray(const Vector3 &position, const Vector3 &direction) :
	_position(position), _direction(direction.normalized()), _objects(nullptr)
{}

void Ray::set(const Vector3 &position, const Vector3 &direction)
{
	_position = position;
	_direction = direction.normalized();
}

void Ray::setObjects(std::vector<Object *> *objects)
{
	_objects = objects;
}

std::vector<Intersection> Ray::cast(const float length) const
{
	std::vector<Intersection> intersections;

	if (!_objects)
	{
		return intersections;
	}

	for (Object *object : *_objects)
	{
		Entity *entity = dynamic_cast<Entity *>(object);
		if (entity)
		{
			float distance = _position.distance(entity->getPosition());
			if (distance <= length)
			{
				intersect(entity, intersections);
			}
		}
	}

	return intersections;
}

void Ray::intersect(Entity *entity, std::vector<Intersection> &intersections) const
{
	Mesh *mesh = entity->getMesh();
	if (!mesh)
	{
		return;
	}
	std::vector<float> *vertices = mesh->getVertices();
	Matrix4 matrix = entity->computeModelMatrix();

	for (int i = 0; i < static_cast<int>(vertices->size()) - 8; i += 9)
	{
		Vector3 a(vertices->at(i), vertices->at(i + 1), vertices->at(i + 2));
		Vector3 b(vertices->at(i + 3), vertices->at(i + 4), vertices->at(i + 5));
		Vector3 c(vertices->at(i + 6), vertices->at(i + 7), vertices->at(i + 8));

		a = matrix.apply(a);
		b = matrix.apply(b);
		c = matrix.apply(c);

		Vector3 *point = intersectTriangle(a, b, c);

		if (point)
		{
			intersections.emplace_back(Intersection({*point, entity}));
		}
	}
}

// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm#C++_implementation
Vector3 *Ray::intersectTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c) const
{
	Vector3 edge1 = b - a;
	Vector3 edge2 = c - a;
	Vector3 ray_cross_e2 = _direction.cross(edge2);
	float det = edge1.dot(ray_cross_e2);

	if (det > -FLT_EPSILON && det < FLT_EPSILON)
	{
		return nullptr; // parallel
	}

	float inv_det = 1.0f / det;
	Vector3 s = _position - a;
	float u = inv_det * s.dot(ray_cross_e2);

	if ((u < 0 && abs(u) > FLT_EPSILON) || (u > 1 && abs(u - 1) > FLT_EPSILON))
	{
		return nullptr;
	}

	Vector3 s_cross_e1 = s.cross(edge1);
	float v = inv_det * _direction.dot(s_cross_e1);

	if ((v < 0 && abs(v) > FLT_EPSILON) || (u + v > 1 && abs(u + v - 1) > FLT_EPSILON))
	{
		return nullptr;
	}

	float t = inv_det * edge2.dot(s_cross_e1);

	if (t > FLT_EPSILON)
	{
		return new Vector3(_position + _direction * t); // intersection
	}

	return nullptr; // line intersection
}

Entity *Ray::getEntityToVisualize(float length) const
{
	Shader *shader = new Shader("assets/shaders/basic_vert.glsl", "assets/shaders/basic_frag.glsl");
	Material *material = new Material();
	material->setWireframe(true);
	material->setRenderType(Material::RenderType::Lines);

	Vector3 endPosition = _position + _direction.normalized(length);

	std::vector<float> vertices = {
		_position.x, _position.y, _position.z,
		endPosition.x, endPosition.y, endPosition.z};
	std::vector<float> colors = {
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f};
	Mesh *mesh = new Mesh(vertices, colors);

	Entity *entity = new Entity(mesh, material, shader);

	return entity;
}
} // namespace Engine
