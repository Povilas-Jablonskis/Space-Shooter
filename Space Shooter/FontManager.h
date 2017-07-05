#ifndef _fontManagerH
#define _fontManagerH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>

namespace Engine
{
	class FontManager
	{
		public:
			FontManager();
			~FontManager();
			void LoadFont(const std::string&, const std::string&);
			FT_Face* GetFont(const std::string&);
		private:
			FT_Library library;
			std::map<std::string, FT_Face> faces;
	};
}
#endif