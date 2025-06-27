#include "Texture.hpp"
#include "../Logging.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Engine
{
Texture::Texture(const std::string &path)
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	DEBUG("Loading " << path);
	unsigned char *data = stbi_load(("assets/textures/" + path).c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		WARN("Failed to load: " << path);
		data = stbi_load("assets/textures/fallback.png", &width, &height, &nrChannels, 0);
	}
	if (data)
	{
		GLint format = GL_RGB;
		if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		else if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 1)
		{
			format = GL_RED;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}

void Texture::use() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
}
} // namespace Engine
