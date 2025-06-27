#pragma once

#include "../UI/Font.hpp"
#include <string>

namespace Engine
{
class BitmapFont : public Font
{
public:
	BitmapFont(const std::string &path);
};
} // namespace Engine
