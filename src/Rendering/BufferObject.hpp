#pragma once

#include <glad/glad.h>
#include <vector>

class BufferObject
{
public:
	BufferObject();
	BufferObject(std::vector<float> *data);
	~BufferObject();

	void setData(std::vector<float> *data);

	void bind(GLuint index, GLint size);
	void refresh();

private:
	GLuint _VBO;
	std::vector<float> *_data;
};
