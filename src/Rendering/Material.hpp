#pragma once

#include "../Rendering/Mesh.hpp"
#include "../Rendering/Shader.hpp"
#include "../Rendering/Texture.hpp"

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
		Quads = GL_QUADS,
	};

	Material();
	~Material();

	void setTexture(Texture *texture)
	{
		_texture = texture;
	}

	void draw(Mesh *mesh, Shader *shader);

	void setRenderType(RenderType type);
	void setWireframe(bool value);
	void setCullBackface(bool value);

private:
	Texture *_texture;
	GLuint _renderType;
	bool _wireframe;
	bool _cullBackface;
};
