#include "Material.hpp"
#include "../Rendering/Texture.hpp"

Material::Material() :
	_texture(nullptr), _renderType(Triangles), _wireframe(false), _cullBackface(true)
{
}
Material::~Material()
{
}

void Material::draw(Mesh *mesh, Shader *shader)
{
	if (_cullBackface)
	{
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	glFrontFace(GL_CW);

	if (_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (_cullBackface)
		{
			glEnable(GL_CULL_FACE);
		}
	}

	if (_texture)
	{
		_texture->use();

		GLint texLoc = glGetUniformLocation(shader->getProgramID(), "tex");
		if (texLoc != -1)
		{
			glUniform1i(texLoc, 0);
		}
	}

	glBindVertexArray(mesh->_VAO);
	glDrawArrays(_renderType, 0, static_cast<GLsizei>(mesh->_vertexCount));
}

void Material::setRenderType(RenderType type)
{
	_renderType = type;
}
void Material::setWireframe(bool value)
{
	_wireframe = value;
}
void Material::setCullBackface(bool value)
{
	_cullBackface = value;
}
