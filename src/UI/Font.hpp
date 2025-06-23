#pragma once

#include <vector>

class Font
{
public:
	Font() = default;
	~Font() = default;

	/*
	 * Returns the vertices for the character at ch - 32
	 */
	std::vector<float> *getCharacter(const char &ch);

protected:
	std::vector<std::vector<float>> _letters;
};
