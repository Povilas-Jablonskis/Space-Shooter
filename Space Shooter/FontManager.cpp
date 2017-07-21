#include "FontManager.h"

namespace Engine
{
	FontManager::FontManager()
	{
		if (FT_Init_FreeType(&library))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FontManager::~FontManager()
	{

	}

	void FontManager::loadFont(const std::string& _path, const std::string& _name)
	{
		if (faces.find(_name) != faces.end())
			return;

		FT_Face face;
		if (FT_New_Face(library, _path.c_str(), 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		auto tempFont = std::make_shared<Font>(face);
		faces.insert(std::pair<std::string, std::shared_ptr<Font>>(_name, tempFont));
	}

	std::shared_ptr<Font> FontManager::getFont(const std::string& name)
	{
		auto tempFace = faces.find(name);
		if (tempFace != faces.end())
			return tempFace->second;
		return nullptr;
	}
}