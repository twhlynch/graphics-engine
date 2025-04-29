#include "OBJLoader.hpp"
#include "../Logging.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

OBJLoader::OBJLoader() {}
OBJLoader::~OBJLoader() {}

Mesh *OBJLoader::parse(const std::string &content)
{
	std::stringstream buffer(content);

	std::vector<float> vertices;
	std::vector<size_t> indices;

	std::string line;
	while (std::getline(buffer, line))
	{
		parseLine(line, vertices, indices);
	}

	std::vector<float> reorderedVertices = reorderVertices(vertices, indices);

	return new Mesh(reorderedVertices);
}

Mesh *OBJLoader::load(const std::string &path)
{
	INFO("Loading " << path);

	std::ifstream file(path);
	if (!file.is_open())
	{
		ERROR("Failed to load: " << path);
		return nullptr;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	return parse(buffer.str());
}

void OBJLoader::parseLine(const std::string &line, std::vector<float> &vertices, std::vector<size_t> &indices)
{
	if (line.empty() || line[0] == '#')
		return;

	std::stringstream ss(line);
	std::string type;
	ss >> type;

	if (type == "v")
	{
		float x, y, z;
		ss >> x >> y >> z;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
	else if (type == "f")
	{
		std::string vertDef;
		std::vector<size_t> faceIndices;

		while (ss >> vertDef)
		{
			std::istringstream vertStream(vertDef);
			std::string indexStr;
			std::getline(vertStream, indexStr, '/');

			if (!indexStr.empty())
				faceIndices.push_back(std::stoi(indexStr) - 1);
		}

		for (size_t i = 1; i + 1 < faceIndices.size(); i++)
		{
			indices.push_back(faceIndices[i + 1]);
			indices.push_back(faceIndices[i]);
			indices.push_back(faceIndices[0]);
		}
	}
}

std::vector<float> OBJLoader::reorderVertices(const std::vector<float> &vertices, const std::vector<size_t> &indices)
{
	std::vector<float> reorderedVertices;
	reorderedVertices.reserve(indices.size() * 3);

	for (size_t index : indices)
	{
		if (index >= 0 && index * 3 + 2 < vertices.size())
		{
			reorderedVertices.push_back(vertices[index * 3]);
			reorderedVertices.push_back(vertices[index * 3 + 1]);
			reorderedVertices.push_back(vertices[index * 3 + 2]);
		}
		else
		{
			ERROR("Invalid vertex index: " << index);

			reorderedVertices.push_back(0.0f);
			reorderedVertices.push_back(0.0f);
			reorderedVertices.push_back(0.0f);
		}
	}

	return reorderedVertices;
}