#pragma once

#include <filesystem>
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
	void refresh();

	GLuint getProgramID() const;

private:
	static std::string loadFile(const std::string &path);
	static GLuint compileShader(GLenum type, const std::string &src);
	static GLuint linkProgram(const std::string &vs, const std::string &fs);

	static bool hasChanged(const std::string &path, std::filesystem::file_time_type &lastTime);

	std::string _vertexPath, _fragmentPath;
	std::filesystem::file_time_type _lastVertexWrite, _lastFragmentWrite;
	GLuint _programID;
};
} // namespace Engine
