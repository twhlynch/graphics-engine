#pragma once

#include "../Logging/Logging.hpp"
#include <fstream>
#include <sstream>
#include <string>

namespace Engine
{
class FileSystem
{
public:
	static std::string *read(const std::string &path)
	{
		INFO("Loading: " << path);

		std::ifstream file(path);

		if (!file)
		{
			ERROR("Failed to load: " << path);
			return nullptr;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();

		return new std::string(buffer.str());
	}
};
} // namespace Engine
