#include "FontLoader.h"

namespace Engine
{
	FontLoader::FontLoader()
	{
		if (FT_Init_FreeType(&library))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FontLoader::~FontLoader()
	{

	}

	void FontLoader::LoadFont(const std::string& _path, const std::string& _name)
	{
		if (faces.find(_name) != faces.end())
			return;

		FT_Face face;
		if (FT_New_Face(library, _path.c_str(), 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		faces.insert(std::pair<std::string, FT_Face>(_name, face));
	}

	const FT_Face FontLoader::GetFont(const std::string& _name)
	{
		if (faces.find(_name) != faces.end())
			return faces.at(_name);
		return NULL;
	}
}