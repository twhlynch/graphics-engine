#include "Material.hpp"

Material::Material() :
	_renderType(Triangles), _wireframe(false)
{
}
Material::~Material()
{
}

void Material::draw(Mesh *mesh)
{
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	if (_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
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