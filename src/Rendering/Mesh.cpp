#include "Mesh.hpp"

#include "../Logging.hpp"

#include <cmath>

Mesh::Mesh(const std::vector<float> vertices, const std::vector<float> colors) :
	_colorsVBO(0), _vertices(vertices), _colors(colors)
{
	_vertexCount = _vertices.size() / 3;
	if (_colors.empty())
		_colors = generateColors(_vertices);

	glGenBuffers(1, &_pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &_colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(float), _colors.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _pointsVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _colorsVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_pointsVBO);
}

std::vector<float> Mesh::generateColors(const std::vector<float> &vertices)
{
	std::vector<float> colors;
	colors.reserve(vertices.size());

	for (size_t i = 0; i < vertices.size(); i++)
	{
		colors.push_back((std::sin(vertices[i]) + 1) * 0.5f);
	}

	return colors;
}

Mesh *Mesh::WithTriangle(float w, float h)
{
	float hw = w * 0.5f;
	float hh = h * 0.5f;

	return new Mesh({
		hw, -hh, 0.0f,	// bottom right
		-hw, -hh, 0.0f, // bottom left
		0.0f, hh, 0.0f, // top middle
	});
}

Mesh *Mesh::WithQuad(float w, float h)
{
	float hw = w * 0.5f;
	float hh = h * 0.5f;

	return new Mesh({
		hw, -hh, 0.0f,	// bottom right
		-hw, -hh, 0.0f, // bottom left
		-hw, hh, 0.0f,	// top left

		hw, hh, 0.0f,  // top right
		hw, -hh, 0.0f, // bottom right
		-hw, hh, 0.0f, // top left
	});
}

Mesh *Mesh::WithCube(float scale)
{
	float hs = scale * 0.5f;

	return new Mesh({
		-hs, -hs, hs, hs, hs, hs, hs, -hs, hs, // front bottom left
		-hs, -hs, hs, -hs, hs, hs, hs, hs, hs, // front top right

		-hs, -hs, -hs, hs, hs, -hs, -hs, hs, -hs, // back bottom left
		-hs, -hs, -hs, hs, -hs, -hs, hs, hs, -hs, // back top right

		-hs, -hs, -hs, -hs, hs, hs, -hs, -hs, hs, // left bottom left
		-hs, -hs, -hs, -hs, hs, -hs, -hs, hs, hs, // left top right

		hs, -hs, -hs, hs, hs, hs, hs, hs, -hs, // right bottom left
		hs, -hs, -hs, hs, -hs, hs, hs, hs, hs, // right top right

		-hs, hs, -hs, hs, hs, hs, -hs, hs, hs, // top bottom left
		-hs, hs, -hs, hs, hs, -hs, hs, hs, hs, // top top right

		-hs, -hs, -hs, hs, -hs, hs, hs, -hs, -hs, // bottom bottom left
		-hs, -hs, -hs, -hs, -hs, hs, hs, -hs, hs, // bottom top right
	});
}

Mesh *Mesh::WithPyramid(float width, float height)
{
	float hw = width * 0.5f;
	float hh = height * 0.5f;

	return new Mesh({
		-hw, -hh, -hw, hw, -hh, hw, hw, -hh, -hw, // base bottom left
		-hw, -hh, -hw, -hw, -hh, hw, hw, -hh, hw, // base top right

		-hw, -hh, -hw, hw, -hh, -hw, -0.0f, hh, -0.0f, // back
		hw, -hh, -hw, hw, -hh, hw, -0.0f, hh, -0.0f,   // right
		hw, -hh, hw, -hw, -hh, hw, -0.0f, hh, -0.0f,   // front
		-hw, -hh, hw, -hw, -hh, -hw, -0.0f, hh, -0.0f  // left
	});
}

Mesh *Mesh::WithCircle(float radius, size_t segments)
{
	std::vector<float> vertices;

	float offset = (2.0f * M_PI) / segments;
	float angle = 0.0f;
	for (size_t i = 0; i < segments; i++)
	{
		vertices.push_back(radius * std::cos(angle));
		vertices.push_back(radius * std::sin(angle));
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		angle += offset;

		vertices.push_back(radius * std::cos(angle));
		vertices.push_back(radius * std::sin(angle));
		vertices.push_back(0.0f);
	}

	return new Mesh(vertices);
}

Mesh *Mesh::WithCylinder(float radius, float height, size_t segments)
{
	std::vector<float> vertices;

	float hh = height * 0.5f;

	float offset = (2.0f * M_PI) / segments;
	float angle = 0.0f;
	for (size_t i = 0; i < segments; i++)
	{
		float nextAngle = angle + offset;
		float startX = radius * std::cos(angle);
		float startZ = radius * std::sin(angle);
		float endX = radius * std::cos(nextAngle);
		float endZ = radius * std::sin(nextAngle);

		// top segment
		vertices.push_back(startX);
		vertices.push_back(hh);
		vertices.push_back(startZ);

		vertices.push_back(endX);
		vertices.push_back(hh);
		vertices.push_back(endZ);

		vertices.push_back(0.0f);
		vertices.push_back(hh);
		vertices.push_back(0.0f);

		// bottom segment
		vertices.push_back(startX);
		vertices.push_back(-hh);
		vertices.push_back(startZ);

		vertices.push_back(0.0f);
		vertices.push_back(-hh);
		vertices.push_back(0.0f);

		vertices.push_back(endX);
		vertices.push_back(-hh);
		vertices.push_back(endZ);

		// quad
		vertices.push_back(endX); // bottom right
		vertices.push_back(-hh);
		vertices.push_back(endZ);

		vertices.push_back(startX); // top left
		vertices.push_back(hh);
		vertices.push_back(startZ);

		vertices.push_back(startX); // bottom left
		vertices.push_back(-hh);
		vertices.push_back(startZ);

		vertices.push_back(endX); // top right
		vertices.push_back(hh);
		vertices.push_back(endZ);

		vertices.push_back(startX); // top left
		vertices.push_back(hh);
		vertices.push_back(startZ);

		vertices.push_back(endX); // bottom right
		vertices.push_back(-hh);
		vertices.push_back(endZ);

		angle = nextAngle;
	}

	return new Mesh(vertices);
}