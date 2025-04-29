#include "Shader.hpp"

#include "../Logging.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) :
	_vertexPath(vertexPath), _fragmentPath(fragmentPath)
{
	_lastVertexWrite = std::filesystem::last_write_time(vertexPath);
	_lastFragmentWrite = std::filesystem::last_write_time(fragmentPath);
	std::string vs = loadFile(vertexPath);
	std::string fs = loadFile(fragmentPath);
	_programID = linkProgram(vs, fs);
}

Shader::~Shader()
{
	glDeleteProgram(_programID);
}

void Shader::use()
{
	glUseProgram(_programID);
}

void Shader::refresh()
{
	bool changed = hasChanged(_vertexPath, _lastVertexWrite) || hasChanged(_fragmentPath, _lastFragmentWrite);

	if (changed)
	{
		INFO("Shaders changed. Reloading...");
		GLuint newProgram = linkProgram(loadFile(_vertexPath), loadFile(_fragmentPath));

		if (newProgram)
		{
			glDeleteProgram(_programID);
			_programID = newProgram;
		}
	}
}

std::string Shader::loadFile(const std::string &path)
{
	INFO("Loading " << path);

	std::ifstream file(path);

	if (!file)
	{
		ERROR("Failed to load: " << path);
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
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

bool Shader::hasChanged(const std::string &path, std::filesystem::file_time_type &lastTime)
{
	auto current = std::filesystem::last_write_time(path);

	if (current != lastTime)
	{
		lastTime = current;
		return true;
	}

	return false;
}

GLuint Shader::getProgramID() const
{
	return _programID;
}