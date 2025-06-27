#pragma once

#include <vector>

namespace Engine
{
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
	void addLetter(std::vector<float> &letter);

private:
	std::vector<std::vector<float>> _letters;
};
} // namespace Engine
