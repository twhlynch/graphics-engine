#include "Entity.hpp"
#include "../Logging/Logging.hpp"
#include "../Math/Matrix4.hpp"
#include <glad/glad.h>

namespace Engine
{
Entity::Entity(Mesh *mesh, Material *material, Shader *shader) :
	_mesh(nullptr), _material(nullptr), _shader(nullptr), _scale(1.0f)
{
	if (mesh)
	{
		_mesh = mesh;
	}
	else
	{
		WARN("Invalid mesh");
	}

	if (material)
	{
		_material = material;
	}
	else
	{
		WARN("Invalid material");
	}

	if (shader)
	{
		_shader = shader;
	}
	else
	{
		WARN("Invalid shader");
	}
}

Entity::~Entity()
{
}

void Entity::setPosition(const Vector3 &position)
{
	_position = position;
}

void Entity::setRotation(const Quaternion &rotation)
{
	_rotation = rotation;
}

void Entity::setScale(const Vector3 &scale)
{
	_scale = scale;
}

Matrix4 Entity::computeModelMatrix() const
{
	Matrix4 translationMatrix = Matrix4::WithTranslation(_position);
	Matrix4 rotationMatrix = Matrix4::WithRotation(_rotation);
	Matrix4 scaleMatrix = Matrix4::WithScale(_scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}

Shader *Entity::getShader() const
{
	return _shader;
}
void Entity::setShader(Shader *shader)
{
	_shader = shader;
}
Material *Entity::getMaterial() const
{
	return _material;
}
void Entity::setMaterial(Material *material)
{
	_material = material;
}
Mesh *Entity::getMesh() const
{
	return _mesh;
}
void Entity::setMesh(Mesh *mesh)
{
	_mesh = mesh;
}

void Entity::update(float /*delta*/)
{
}

void Entity::draw()
{
	if (!_shader)
	{
		ERROR("Cannot draw without shader!");
		return;
	}

	_shader->use();

	GLint modelLoc = glGetUniformLocation(_shader->getProgramID(), "model");

	if (modelLoc != -1)
	{
		Matrix4 model = computeModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
	}
	else
	{
		WARN("'model' uniform not found in shader.");
	}

	_material->draw(_mesh, _shader);
}
} // namespace Engine
