#include "BufferObject.hpp"

namespace Engine
{
BufferObject::BufferObject() :
	_vbo(0), _data(nullptr)
{
	glGenBuffers(1, &_vbo);
}

BufferObject::BufferObject(std::vector<float> *data) :
	_vbo(0), _data(data)
{
	glGenBuffers(1, &_vbo);

	refresh();
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &_vbo);
}

void BufferObject::setData(std::vector<float> *data)
{
	_data = data;

	refresh();
}

void BufferObject::bind(GLuint index, GLint size) const
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, size * static_cast<int>(sizeof(float)), (void *)0);
	glEnableVertexAttribArray(index);
}

void BufferObject::refresh()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long>(_data->size()) * static_cast<long>(sizeof(float)), _data->data(), GL_STATIC_DRAW);
}
} // namespace Engine
