#include "ConfigurationManager.h"

namespace Engine
{
	ConfigurationManager::~ConfigurationManager()
	{
		auto characterList = interfaceFont->getCharacterList();
		for (auto it2 = characterList->begin(); it2 != characterList->end(); ++it2)
		{
			glDeleteTextures(1, &it2->second.TextureID);
		}
	}

	ConfigurationManager::ConfigurationManager()
	{
		FT_Library library;

		if (FT_Init_FreeType(&library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			#endif
		}

		FT_Face face;
		if (FT_New_Face(library, "Fonts/kenvector_future_thin.ttf", 0, &face))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			#endif
		}

		interfaceFont = std::make_shared<Font>(std::move(face));

		// We Don't Need The Face Information Now That The Display
		// Lists Have Been Created, So We Free The Assosiated Resources.
		FT_Done_Face(face);

		// Ditto For The Font Library.
		FT_Done_FreeType(library);
	}
}