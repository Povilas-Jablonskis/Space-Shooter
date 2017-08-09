#ifndef _fontManagerH
#define _fontManagerH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>

#include "Font.h"

namespace Engine
{
	class FontManager
	{
		public:
			~FontManager();
			FontManager();
			void loadFont(const std::string&, const std::string&);
			std::shared_ptr<Font> getFont(const std::string&);
		private:
			FT_Library library;
			std::map<std::string, std::shared_ptr<Font>> faces;
	};
}
#endif