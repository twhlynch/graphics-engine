#pragma once

#include "../Math/Matrix4.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Vector3.hpp"
#include "../Rendering/Material.hpp"
#include "../Rendering/Mesh.hpp"
#include "../Rendering/Shader.hpp"

class Entity
{
public:
	Entity(Mesh *mesh, Material *material, Shader *shader);
	virtual ~Entity();

	void setPosition(const Vector3 &position);
	void setRotation(const Quaternion &rotation);
	void setScale(const Vector3 &scale);

	Vector3 getPosition() const
	{
		return _position;
	}
	Quaternion getRotation() const
	{
		return _rotation;
	}
	Vector3 getScale() const
	{
		return _scale;
	}

	Shader *getShader() const;
	void setShader(Shader *shader);
	Material *getMaterial() const;
	void setMaterial(Material *material);
	Mesh *getMesh() const;
	void setMesh(Mesh *mesh);

	virtual void draw();
	virtual void update(float delta);

	Matrix4 computeModelMatrix() const;

private:
	Mesh *_mesh;
	Material *_material;
	Shader *_shader;

	Vector3 _position;
	Quaternion _rotation;
	Vector3 _scale;
};
