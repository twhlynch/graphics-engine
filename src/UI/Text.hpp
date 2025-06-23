#pragma once

#include <glad/glad.h>

#include "../Rendering/Mesh.hpp"
#include "../UI/Font.hpp"

class Text : public Mesh
{
public:
	Text(Font *font);
	~Text();

	void setText(const std::string &text);

	Font *_font;
};
