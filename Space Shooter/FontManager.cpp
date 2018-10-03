#include "FontManager.h"

namespace Engine
{
	FontManager::~FontManager()
	{
		for (auto it = faces.begin(); it != faces.end(); ++it)
		{
			auto characterList = it->second->getCharacterList();
			for (auto it2 = characterList->begin(); it2 != characterList->end(); ++it2)
			{
				glDeleteTextures(1, &it2->second.TextureID);
			}
		}

		faces.clear();

		if (FT_Done_FreeType(library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not free FreeType Library" << std::endl;
			#endif
		}
	}

	FontManager::FontManager()
	{
		if (FT_Init_FreeType(&library))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			#endif
		}
	}

	void FontManager::loadFont(std::string _path, std::string _name)
	{
		for (auto face : faces)
		{
			if (face.first == _name)
				return;
		}

		FT_Face face;
		if (FT_New_Face(library, ("Fonts/" + _path).c_str(), 0, &face))
		{
			#if _DEBUG
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			#endif
		}
		faces.push_back(font(_name, std::move(std::make_shared<Font>(face))));
	}

	std::shared_ptr<Font> FontManager::getFont(std::string name)
	{
		for (auto face : faces)
		{
			if (face.first == name)
				return face.second;
		}

		return nullptr;
	}
}