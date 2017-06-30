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
			FontLoader();
			~FontLoader();
			void LoadFont(const std::string&, const std::string&);
			const FT_Face GetFont(const std::string&);
		private:
			FT_Library library;
			std::map<std::string, FT_Face> faces;
		};
}
#endif