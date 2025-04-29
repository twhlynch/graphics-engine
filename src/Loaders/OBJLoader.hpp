#pragma once

#include "../Rendering/Mesh.hpp"

#include <string>
#include <vector>

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	static Mesh *load(const std::string &path);
	static Mesh *parse(const std::string &content);

private:
	static void parseLine(const std::string &line, std::vector<float> &vertices, std::vector<size_t> &indices);
	static std::vector<float> reorderVertices(const std::vector<float> &vertices, const std::vector<size_t> &indices);
};