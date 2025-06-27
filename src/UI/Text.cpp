#include "Text.hpp"

namespace Engine
{
Text::Text(Font *font) :
	Mesh({}), _font(font)
{
}

Text::~Text()
{
}

void Text::setText(const std::string &text)
{
	std::vector<float> vertices;
	std::vector<float> colors;

	float xOffset = 0;
	float yOffset = 0;

	float xExtent = 0;
	float yExtent = 0;

	for (size_t i = 0; i < text.size(); i++)
	{
		int ch = static_cast<unsigned char>(text.at(i));

		if (ch == '\n')
		{
			yOffset += 0.6f;
			xOffset = 0;
			continue;
		}

		std::vector<float> *character = _font->getCharacter(static_cast<char>(ch));

		for (size_t j = 0; j < character->size(); j += 3)
		{
			float x = character->at(j) + xOffset;
			float y = character->at(j + 1) - yOffset;

			vertices.emplace_back(x);
			vertices.push_back(y);
			vertices.push_back(character->at(j + 2));

			colors.emplace_back(1);
			colors.emplace_back(1);
			colors.emplace_back(1);

			xExtent = std::max(xExtent, x);
			yExtent = std::max(yExtent, y);
		}

		xOffset += 0.6f;
	}

	yExtent *= 0.5f;
	xExtent *= 0.5f;
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		vertices.at(i) -= xExtent;
		vertices.at(i + 1) += yExtent;
	}

	setVertices(vertices);
	setColors(colors);
	changed();
}
} // namespace Engine
