#include "FontLoader.h"

namespace Engine
{
	std::map<std::string, FT_Face> FontLoader::faces;
	FT_Library FontLoader::library;

	void FontLoader::Init()
	{
		if (FT_Init_FreeType(&library))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	void FontLoader::LoadFont(std::string _path, std::string _name)
	{
		if (faces.find(_name) != faces.end())
			return;

		FT_Face face;
		if (FT_New_Face(library, _path.c_str(), 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		faces.insert(std::pair<std::string, FT_Face>(_name, face));
	}

	FT_Face FontLoader::GetFont(std::string _name)
	{
		if (faces.find(_name) != faces.end())
			return faces.at(_name);
		return NULL;
	}
}