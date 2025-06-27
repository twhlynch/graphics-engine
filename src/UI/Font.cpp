#include "Font.hpp"

namespace Engine
{
std::vector<float> *Font::getCharacter(const char &ch)
{
	return &_letters.at(ch - 32);
}

void Font::addLetter(std::vector<float> &letter)
{
	_letters.emplace_back(letter);
}
} // namespace Engine
