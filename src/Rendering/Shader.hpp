#pragma once

#include <glad/glad.h>
#include <string>

namespace Engine
{
class Shader
{
public:
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	~Shader();

	void use() const;

	GLuint getProgramID() const;

private:
	static GLuint compileShader(GLenum type, const std::string &src);
	static GLuint linkProgram(const std::string &vs, const std::string &fs);

	std::string _vertexPath, _fragmentPath;
	GLuint _programID;
};
} // namespace Engine
