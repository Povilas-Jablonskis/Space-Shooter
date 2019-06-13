#ifndef _configurationManagerH
#define _configurationManagerH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <iostream>

#include "Font.h"

namespace Engine
{
	class ConfigurationManager
	{
		public:
			~ConfigurationManager();
			ConfigurationManager();
			inline std::shared_ptr<Font> getInterfaceFont() { return interfaceFont; }
		private:
			std::shared_ptr<Font> interfaceFont;
	};
}
#endif