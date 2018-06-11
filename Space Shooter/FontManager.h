#ifndef _fontManagerH
#define _fontManagerH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>
#include <vector>

#include "Font.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Font>> font;

	class FontManager
	{
		public:
			~FontManager();
			FontManager();
			void loadFont(const std::string&, const std::string&);
			std::shared_ptr<Font> getFont(const std::string&);
			inline std::vector<font>* getFonts() { return &faces; };
		private:
			FT_Library library;
			std::vector<font> faces;
	};
}
#endif