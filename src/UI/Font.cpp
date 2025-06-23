#include "../UI/Font.hpp"

std::vector<float> *Font::getCharacter(const char &ch)
{
	return &_letters.at(ch - 32);
}
