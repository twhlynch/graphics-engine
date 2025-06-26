#include "Mesh.hpp"

#include <cmath>

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<float> &colors, const std::vector<float> &texCoords) :
	_vertices(vertices), _colors(colors), _texCoords(texCoords)
{
	_vertexCount = _vertices.size() / 3;
	if (_colors.empty())
	{
		_colors = generateColors(_vertices);
	}
	if (_texCoords.empty())
	{
		_texCoords = generateTexCoords(_vertices);
	}

	_pointsVBO.setData(&_vertices);
	_colorsVBO.setData(&_colors);
	_texCoordsVBO.setData(&_texCoords);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	_pointsVBO.bind(0, 3);
	_colorsVBO.bind(1, 3);
	_texCoordsVBO.bind(2, 2);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::refresh()
{
	if (_changed)
	{
		_changed = false;

		_pointsVBO.refresh();
		_colorsVBO.refresh();
		_texCoordsVBO.refresh();

		_vertexCount = _vertices.size() / 3;
	}
}

Mesh *Mesh::operator+(const Mesh &other) const
{
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> colors;

	vertices.insert(vertices.end(), _vertices.begin(), _vertices.end());
	vertices.insert(vertices.end(), other._vertices.begin(), other._vertices.end());
	colors.insert(colors.end(), _colors.begin(), _colors.end());
	colors.insert(colors.end(), other._colors.begin(), other._colors.end());
	texCoords.insert(texCoords.end(), _texCoords.begin(), _texCoords.end());
	texCoords.insert(texCoords.end(), other._texCoords.begin(), other._texCoords.end());

	return new Mesh(vertices, colors, texCoords);
}

std::vector<float> Mesh::generateTexCoords(const std::vector<float> &vertices)
{
	std::vector<float> texCoords;

	size_t vertexCount = vertices.size() / 3;
	for (size_t i = 0; i < vertexCount; i += 6)
	{ // triangles only
		std::vector<float> faceCoords = {
			0.0f, 0.0f, // bottom left
			1.0f, 1.0f, // top right
			0.0f, 1.0f, // top left

			0.0f, 0.0f, // bottom left
			1.0f, 0.0f, // bottom right
			1.0f, 1.0f, // top right
		};
		texCoords.insert(texCoords.end(), faceCoords.begin(), faceCoords.end());
	}

	return texCoords;
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

	// clang-format off
	return new Mesh({
		-hw, -hh, 0.0f, // bottom left
		hw, hh, 0.0f,   // top right
		hw, -hh, 0.0f,  // bottom right

		-hw, -hh, 0.0f, // bottom left
		-hw,hh, 0.0f,   // top left
		hw,  hh, 0.0f,  // top right
	}, {}, {
		0.0f, 1.0f, // top left
		1.0f, 0.0f, // bottom right
		1.0f, 1.0f, // top right

		0.0f, 1.0f, // top left
		0.0f, 0.0f, // bottom left
		1.0f, 0.0f, // top left
	});
	// clang-format on
}

Mesh *Mesh::WithCube(float scale)
{
	float hs = scale * 0.5f;

	// clang-format off
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
	}, {}, {
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	});
	// clang-format on
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

	float offset = (2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments);
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

	float offset = (2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments);
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
