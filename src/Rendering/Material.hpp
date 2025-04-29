#pragma once

#include "../Rendering/Mesh.hpp"

#include <glad/glad.h>

class Material
{
public:
	enum RenderType
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineLoop = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP,
		Triangles = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
	};

	Material();
	~Material();

	void draw(Mesh *mesh);

	void setRenderType(RenderType type);
	void setWireframe(bool value);

private:
	GLuint _renderType;
	bool _wireframe;
};