#include "Ray.hpp"

#include "../Rendering/Material.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Shader.hpp"

Ray::Ray() {}
Ray::Ray(const Vector3 &position, const Vector3 &direction) :
	_position(position), _direction(direction.normalized())
{}

void Ray::set(const Vector3 &position, const Vector3 &direction)
{
	_position = position;
	_direction = direction.normalized();
}

void Ray::setObjects(const std::vector<Entity *> &objects)
{
	_objects = objects;
}

std::vector<Intersection> Ray::cast(const float /*length*/) const
{
	std::vector<Intersection> intersections;

	// TODO:

	return intersections;
}

Entity *Ray::getEntityToVisualize(const float length) const
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