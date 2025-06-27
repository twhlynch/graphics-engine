#include "BitmapFont.hpp"
#include "../Logging.hpp"
#include <fstream>
#include <sstream>

namespace Engine
{
BitmapFont::BitmapFont(const std::string &path)
{
	DEBUG("Loading " << path);
	std::ifstream file(path);
	if (!file)
	{
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	std::vector<std::vector<bool>> bitmap;

	std::string line;
	while (getline(buffer, line))
	{
		if (line.empty())
		{
			std::vector<float> vertices;

			std::vector<std::vector<bool>> visited(bitmap.size(), std::vector<bool>(bitmap.at(0).size(), false));

			for (int j = 0; j < static_cast<int>(bitmap.size()); j++)
			{
				for (int i = 0; i < static_cast<int>(bitmap.at(j).size()); i++)
				{
					int y = j, x = i;
					int width = 0, height = 0;

					while (
						x + width < static_cast<int>(bitmap.at(y).size()) &&
						bitmap.at(y).at(x + width) &&
						!visited.at(y).at(x + width))
					{
						visited.at(y).at(x + width) = true;
						width++;
					}

					if (width != 0)
					{
						for (int k = x; k < x + width; k++)
						{
							visited.at(y + height).at(k) = true;
						}
						height++;
					}

					while (
						y + height < static_cast<int>(bitmap.size()) &&
						bitmap.at(y + height).at(x) &&
						!visited.at(y + height).at(x))
					{
						bool invalid = false;
						for (int k = x; k < x + width; k++)
						{
							if (
								visited.at(y + height).at(k) ||
								!bitmap.at(y + height).at(k))
							{
								invalid = true;
							}
						}
						if (invalid)
						{
							break;
						}

						for (int k = x; k < x + width; k++)
						{
							visited.at(y + height).at(k) = true;
						}
						height++;
					}

					if (width != 0 && height != 0)
					{
						std::vector<float> rect = {
							static_cast<float>(x), -static_cast<float>(y), 0,
							static_cast<float>(x) + static_cast<float>(width), -static_cast<float>(y) - static_cast<float>(height), 0,
							static_cast<float>(x), -static_cast<float>(y) - static_cast<float>(height), 0,

							static_cast<float>(x), -static_cast<float>(y), 0,
							static_cast<float>(x) + static_cast<float>(width), -static_cast<float>(y), 0,
							static_cast<float>(x) + static_cast<float>(width), -static_cast<float>(y) - static_cast<float>(height), 0};

						for (size_t i = 0; i < rect.size(); i++)
						{
							rect.at(i) *= 0.1f;
						}

						vertices.insert(vertices.end(), rect.begin(), rect.end());
					}
				}
			}

			bitmap.erase(bitmap.begin(), bitmap.end());
			addLetter(vertices);
		}
		else
		{
			std::vector<bool> linemap;
			for (size_t i = 0; i < line.size(); i++)
			{
				char ch = line.at(i);
				if (ch == ' ')
				{
					linemap.emplace_back(false);
				}
				else
				{
					linemap.emplace_back(true);
				}
			}
			bitmap.emplace_back(linemap);
		}
	}
}
} // namespace Engine
