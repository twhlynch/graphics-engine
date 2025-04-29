#include "Entity.hpp"

#include "../Logging.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Entity::Entity(Mesh *mesh, Material *material, Shader *shader) :
	_mesh(nullptr), _material(nullptr), _shader(nullptr), _scale(1.0f)
{
	if (mesh)
		_mesh = mesh;
	else
		WARN("Invalid mesh");

	if (material)
		_material = material;
	else
		WARN("Invalid material");

	if (shader)
		_shader = shader;
	else
		WARN("Invalid shader");
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

glm::mat4 Entity::computeModelMatrix() const
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position.asGLM());
	glm::mat4 rotationMatrix = glm::mat4_cast(_rotation.asGLM());
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale.asGLM());

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
		glm::mat4 model = computeModelMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}
	else
		WARN("'model' uniform not found in shader.");

	_material->draw(_mesh);
}
