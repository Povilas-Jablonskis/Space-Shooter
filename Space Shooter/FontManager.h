#ifndef _fontManagerH
#define _fontManagerH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>
#include <unordered_map>

namespace Engine
{
	class FontManager
	{
		public:
			FontManager();
			~FontManager();
			void loadFont(const std::string&, const std::string&);
			FT_FaceRec_* getFont(const std::string&);
		private:
			FT_Library library;
			std::unordered_map<std::string, FT_FaceRec_> faces;
	};
}
#endif