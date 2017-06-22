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
			static void LoadFont(const char*);
			static FT_Face GetFont(const char* _path);
		private:
			static FT_Library library;
			static std::map<const char*, FT_Face> faces;
		};
}
#endif