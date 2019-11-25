#ifndef fontH
#define fontH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vec2.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Engine
{
	struct Character
	{
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		FT_Pos     Advance;    // Offset to advance to next glyph
	};

	typedef std::pair<GLchar, Character> character;

	class Font
	{
		public:
			Font(const FT_Face&);
			Character getCharacter(GLchar);
			inline const std::vector<character>& getCharacterList() const { return m_characters; }
		private:
			inline const FT_Face& getFace() const { return m_face; }

			FT_Face m_face;
			void loadCharacters();
			std::vector<character> m_characters;
	};
}
#endif