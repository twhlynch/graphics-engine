#pragma once

#include <glad/glad.h>

#include <vector>

class Material;
class Mesh
{
	friend class Material;

public:
	Mesh(const std::vector<float> vertices, const std::vector<float> colors = {}, const std::vector<float> texCoords = {});
	~Mesh();

	std::vector<float> *GetVertices()
	{
		return &_vertices;
	};

	Mesh *operator+(const Mesh &other) const;

	static std::vector<float> generateColors(const std::vector<float> &vertices);
	static std::vector<float> generateTexCoords(const std::vector<float> &vertices);

	void changed()
	{
		_changed = true;
	}

	void refresh();

	static Mesh *WithTriangle(float w = 1.0f, float h = 1.0f);
	static Mesh *WithQuad(float w = 1.0f, float h = 1.0f);
	static Mesh *WithCube(float scale = 1.0f);
	static Mesh *WithPyramid(float width = 1.0f, float height = 1.0f);
	static Mesh *WithCircle(float radius = 0.5f, size_t segments = 16);
	static Mesh *WithCylinder(float radius = 0.5f, float height = 1.0f, size_t segments = 16);

private:
	GLuint _VAO, _pointsVBO, _colorsVBO, _UVVBO;
	size_t _vertexCount;
	std::vector<float> _vertices;
	std::vector<float> _colors;
	std::vector<float> _texCoords;
	bool _changed;
};
