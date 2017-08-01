#ifndef _fontH
#define _fontH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <memory>

#include "Texture.h"
#include "Character.h"

namespace Engine
{
	class Font
	{
		public:
			Font(FT_Face);
			~Font();
			std::map<GLchar, Character> getCharacterList() { return characters; }
			FT_Face face;
		private:
			void LoadCharacters();
			std::map<GLchar, Character> characters;
	};
}
#endif