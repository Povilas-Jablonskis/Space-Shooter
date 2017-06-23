#ifndef _FontLoader
#define _FontLoader

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>

namespace Engine
{
	class FontLoader
	{
		public:
			static void LoadFont(std::string, std::string);
			static FT_Face GetFont(std::string);
			static void Init();
		private:
			static FT_Library library;
			static std::map<std::string, FT_Face> faces;
		};
}
#endif