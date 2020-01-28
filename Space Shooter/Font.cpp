#include "Font.hpp"

#include <iostream>
#include <algorithm>

namespace Engine
{
	Font::Font(const FT_Face& face) : m_face(face)
	{
		loadCharacters();
	}

	Character Font::getCharacter(GLchar index) const
	{
		auto characters = getCharacterList();
		const auto it = std::find_if(characters.begin(), characters.end(), [index](auto idx) { return idx.first == index; });

		return it != characters.end() ? it->second : Character{ 0, glm::ivec2(0, 0), glm::ivec2(0, 0), 0 };
	}

	void Font::loadCharacters()
	{
		FT_Set_Pixel_Sizes(m_face, 0, 12);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; ++c)
		{
			// Load character glyph 
			if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
			{
				#if _DEBUG
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
				#endif
				continue;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				getFace()->glyph->bitmap.width,
				getFace()->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				getFace()->glyph->bitmap.buffer
				);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character _character = {
				texture,
				glm::ivec2(getFace()->glyph->bitmap.width, getFace()->glyph->bitmap.rows),
				glm::ivec2(getFace()->glyph->bitmap_left, getFace()->glyph->bitmap_top),
				getFace()->glyph->advance.x
			};
			m_characters.emplace_back(c, _character);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}