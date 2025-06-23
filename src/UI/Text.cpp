#include "../UI/Text.hpp"

Text::Text(Font *font) :
	Mesh({}), _font(font)
{
}

Text::~Text()
{
}

void Text::setText(const std::string &text)
{
	_vertices.clear();
	_colors.clear();

	float xOffset = 0;
	float yOffset = 0;

	float xExtent = 0;
	float yExtent = 0;

	for (size_t i = 0; i < text.size(); i++)
	{
		int ch = text.at(i);

		if (ch == '\n')
		{
			yOffset += 0.6f;
			xOffset = 0;
			continue;
		}

		std::vector<float> *character = _font->getCharacter(ch);

		for (size_t j = 0; j < character->size(); j += 3)
		{
			float x = character->at(j) + xOffset;
			float y = character->at(j + 1) - yOffset;

			_vertices.emplace_back(x);
			_vertices.push_back(y);
			_vertices.push_back(character->at(j + 2));

			_colors.emplace_back(1);
			_colors.emplace_back(1);
			_colors.emplace_back(1);

			xExtent = std::max(xExtent, x);
			yExtent = std::max(yExtent, y);
		}

		xOffset += 0.6f;
	}

	yExtent *= 0.5f;
	xExtent *= 0.5f;
	for (size_t i = 0; i < _vertices.size(); i += 3)
	{
		_vertices.at(i) -= xExtent;
		_vertices.at(i + 1) += yExtent;
	}

	changed();
}
