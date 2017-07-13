#include "TextureManager.h"

namespace Engine
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	void TextureManager::loadTexture(const std::string& _path, const std::string& _name, glm::vec2 _animsc )
	{
		if (textures.find(_name) == textures.end())
		{
			auto tempTexture = Texture();
			tempTexture.readTextureFile(_path);
			tempTexture.setCount(_animsc);
			textures.insert(std::pair<std::string, Texture>(_name, tempTexture));
		}
	}

	Texture TextureManager::getTexture(const std::string& _name)
	{
		if (textures.find(_name) != textures.end())
			return textures.find(_name)->second;
		return Texture();
	}

	void TextureManager::setAnimationCount(const std::string& _name, glm::vec2 count)
	{
		if (textures.find(_name) != textures.end())
			textures.find(_name)->second.setCount(count);
	}
}