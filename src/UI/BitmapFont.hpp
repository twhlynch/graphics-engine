#pragma once

#include <string>

#include "../UI/Font.hpp"

class BitmapFont : public Font
{
public:
	BitmapFont(const std::string &path);
};
