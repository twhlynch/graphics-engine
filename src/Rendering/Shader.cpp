#include "Shader.hpp"
#include "../FileSystem/FileSystem.hpp"
#include "../Logging.hpp"

namespace Engine
{
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) :
	_vertexPath(vertexPath), _fragmentPath(fragmentPath)
{
	std::string *vs = FileSystem::read(vertexPath);
	std::string *fs = FileSystem::read(fragmentPath);
	_programID = linkProgram(*vs, *fs);
	delete vs;
	delete fs;
}

Shader::~Shader()
{
	glDeleteProgram(_programID);
}

void Shader::use() const
{
	glUseProgram(_programID);
}

GLuint Shader::compileShader(GLenum type, const std::string &src)
{
	GLuint shader = glCreateShader(type);
	const char *cstr = src.c_str();
	glShaderSource(shader, 1, &cstr, nullptr);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		ERROR("Shader compilation error: " << log);
	}

	return shader;
}

GLuint Shader::linkProgram(const std::string &vs, const std::string &fs)
{
	GLuint vertex = compileShader(GL_VERTEX_SHADER, vs);
	GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fs);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(program, 512, nullptr, log);
		ERROR("Shader linking error: " << log);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

GLuint Shader::getProgramID() const
{
	return _programID;
}
} // namespace Engine
