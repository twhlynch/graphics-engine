#pragma once

#include "../Rendering/Mesh.hpp"
#include "../UI/Font.hpp"
#include <glad/glad.h>

namespace Engine
{
class Text : public Mesh
{
public:
	Text(Font *font);
	~Text();

	void setText(const std::string &text);

	Font *_font;
};
} // namespace Engine
