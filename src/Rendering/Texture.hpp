#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

	void use() const;

private:
	GLuint _texture;
};
