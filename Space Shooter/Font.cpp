#include "Font.hpp"

#include <iostream>

Font::Font(const std::string& path)
{
	FT_Library library;

	if (FT_Init_FreeType(&library))
	{
#if _DEBUG
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
#endif
	}

	FT_Face face;
	if (FT_New_Face(library, path.c_str(), 0, &face))
	{
#if _DEBUG
		std::cout << "ERROR::FREETYPE: Failed to load font\n";
#endif
	}

	FT_Set_Pixel_Sizes(face, 0, 18);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (FT_ULong c = 0; c < 128; ++c)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
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
			static_cast<GLsizei>(face->glyph->bitmap.width),
			static_cast<GLsizei>(face->glyph->bitmap.rows),
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters.insert_or_assign(c, character);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// We Don't Need The Face Information Now That The Display
	// Lists Have Been Created, So We Free The Associated Resources.
	FT_Done_Face(face);

	// Ditto For The Font Library.
	FT_Done_FreeType(library);
}

Character Font::getCharacter(const FT_ULong index) const
{
	return m_characters.at(index);
}
