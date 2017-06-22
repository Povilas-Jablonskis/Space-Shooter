#include "FontLoader.h"

namespace Engine
{
	std::map<const char*, FT_Face> FontLoader::faces;
	FT_Library FontLoader::library;

	void FontLoader::LoadFont(const char* _path)
	{
		if (faces.find(_path) != faces.end())
			return;

		FT_Face face;
		if (library == NULL)
		{
			if (FT_Init_FreeType(&library))
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}
		if (FT_New_Face(library, _path, 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		faces.insert(std::pair<const char*, FT_Face>(_path, face));
	}

	FT_Face FontLoader::GetFont(const char* _path)
	{
		if (faces.find(_path) != faces.end())
			return faces.at(_path);
		return NULL;
	}
}