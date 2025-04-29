#pragma once

#include "../Math/Quaternion.hpp"
#include "../Math/Vector3.hpp"
#include "../Rendering/Material.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Shader.hpp"

class Entity
{
public:
	Entity(Mesh *mesh, Material *material, Shader *shader);

	void setPosition(const Vector3 &position);
	void setRotation(const Quaternion &rotation);
	void setScale(const Vector3 &scale);

	Shader *getShader() const;
	void setShader(Shader *shader);
	Material *getMaterial() const;
	void setMaterial(Material *material);
	Mesh *getMesh() const;
	void setMesh(Mesh *mesh);

	void draw();

private:
	glm::mat4 computeModelMatrix() const;

	Mesh *_mesh;
	Material *_material;
	Shader *_shader;

	Vector3 _position;
	Quaternion _rotation;
	Vector3 _scale;
};
