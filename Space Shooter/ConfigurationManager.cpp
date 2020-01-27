#include "ConfigurationManager.hpp"
#include "Font.hpp"

namespace Engine
{
	ConfigurationManager::ConfigurationManager()
	{
		FT_Library library;

		if (FT_Init_FreeType(&library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
			#endif
		}

		FT_Face face;
		if (FT_New_Face(library, "Fonts/kenvector_future_thin.ttf", 0, &face))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Failed to load font\n";
			#endif
		}

		setInterfaceFont(std::make_shared<Font>(face));

		// We Don't Need The Face Information Now That The Display
		// Lists Have Been Created, So We Free The Assosiated Resources.
		FT_Done_Face(face);

		// Ditto For The Font Library.
		FT_Done_FreeType(library);
	}
}