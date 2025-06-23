#include "BufferObject.hpp"

BufferObject::BufferObject() :
	_VBO(0), _data(nullptr)
{
	glGenBuffers(1, &_VBO);
}

BufferObject::BufferObject(std::vector<float> *data) :
	_VBO(0), _data(data)
{
	glGenBuffers(1, &_VBO);

	refresh();
}

BufferObject::~BufferObject()
{
	glDeleteBuffers(1, &_VBO);
}

void BufferObject::setData(std::vector<float> *data)
{
	_data = data;

	refresh();
}

void BufferObject::bind(GLuint index, GLint size)
{
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void *)0);
	glEnableVertexAttribArray(index);
}

void BufferObject::refresh()
{
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _data->size() * sizeof(float), _data->data(), GL_STATIC_DRAW);
}
