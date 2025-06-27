#pragma once

#include <glad/glad.h>
#include <string>

namespace Engine
{
class Texture
{
public:
	Texture(const std::string &path);
	~Texture();

	void use() const;

private:
	GLuint _texture;
};
} // namespace Engine
