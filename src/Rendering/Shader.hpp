#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <string>

class Shader
{
public:
	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	~Shader();

	void use();
	void refresh();

	GLuint getProgramID() const;

private:
	std::string loadFile(const std::string &path);
	GLuint compileShader(GLenum type, const std::string &src);
	GLuint linkProgram(const std::string &vs, const std::string &fs);

	bool hasChanged(const std::string &path, std::filesystem::file_time_type &lastTime);

	std::string _vertexPath, _fragmentPath;
	std::filesystem::file_time_type _lastVertexWrite, _lastFragmentWrite;
	GLuint _programID;
};
