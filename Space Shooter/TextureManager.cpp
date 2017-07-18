#include "TextureManager.h"

namespace Engine
{
	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	void TextureManager::loadTexture(const std::string& _path, const std::string& _name, int _startFame, int _endFrame, glm::vec2 _animsc)
	{
		if (textures.find(_name) == textures.end())
		{
			auto tempTexture = std::make_shared<Texture>(_startFame, _endFrame, _animsc);
			tempTexture->readTextureFile(_path);
			textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(_name, tempTexture));
		}
	}

	std::shared_ptr<Texture> TextureManager::getTexture(const std::string& _name)
	{
		if (textures.find(_name) != textures.end())
			return textures.find(_name)->second;
		return nullptr;
	}
}