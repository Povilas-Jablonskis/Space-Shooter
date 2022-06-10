#include "Font.hpp"

#include <iostream>
#include <algorithm>

Font::Font(const FT_Face& face) : m_face(face)
{
	FT_Set_Pixel_Sizes(m_face, 0, 18);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (FT_ULong c = 0; c < 128; ++c)
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
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character _character = {
			texture,
			glm::vec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
			glm::vec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
			m_face->glyph->advance.x
		};
		m_characters.insert_or_assign(c, _character);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

Character Font::getCharacter(FT_ULong index) const
{
	return m_characters.at(index);
}