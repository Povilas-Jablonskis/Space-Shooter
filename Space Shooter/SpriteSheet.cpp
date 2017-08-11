#include "SpriteSheet.h"
#include <fstream>

namespace Engine
{
	void SpriteSheet::loadSpriteSheet(const std::string& _path)
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		unsigned char* image = SOIL_load_image(_path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		sprites.insert(std::pair<std::string, glm::vec4>("wholeSpriteSheet", glm::vec4(width, height, width, height)));
	}

	void SpriteSheet::loadSpritesFromXml(const std::string& _path)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile(_path);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("TextureAtlas");
		// Iterate over the brewerys
		for (rapidxml::xml_node<> * brewery_node = root_node->first_node("SubTexture"); brewery_node; brewery_node = brewery_node->next_sibling())
		{
			auto animation = glm::vec4(atof(brewery_node->first_attribute("x")->value()), atof(brewery_node->first_attribute("y")->value()), atof(brewery_node->first_attribute("width")->value()), atof(brewery_node->first_attribute("height")->value()));
			sprites.insert(std::pair<std::string, glm::vec4>(brewery_node->first_attribute("name")->value(), animation));
		}
	}

	std::shared_ptr<Animation> SpriteSheet::getSprite(const std::string& index)
	{
		auto sprite = sprites.find(index);
		if (sprite == sprites.end())
			return nullptr;

		std::shared_ptr<Animation> animation = std::make_shared<Animation>(texture, width, height);
		animation->addSprite(sprite->second);
		return animation;
	}

	glm::vec4 SpriteSheet::getSpriteAsVector(const std::string& index)
	{
		auto sprite = sprites.find(index);
		if (sprite == sprites.end())
			return glm::vec4(0.0,0.0,0.0,0.0);

		return sprite->second;
	}

	int SpriteSheet::getSize(int index) const
	{
		switch (index)
		{
			case 0:
			{
				return width;
			}
			case 1:
			{
				return height;
			}
		}
		return NULL;
	}

	void SpriteSheet::makeAnimation(const std::string& index, std::vector<std::string> indexes)
	{
		if (animations.find(index) != animations.end())
			return;

		std::shared_ptr<Animation> animation = std::make_shared<Animation>(texture, width, height);

		for (auto _index : indexes)
		{
			auto sprite = sprites.find(_index);
			if (sprite != sprites.end())
				animation->addSprite(sprite->second);
		}

		animations.insert(std::pair<std::string, std::shared_ptr<Animation>>(index, animation));
	}

	void SpriteSheet::makeAnimation(const std::string& index, std::vector<glm::vec4> sprites)
	{
		if (animations.find(index) != animations.end())
			return;

		std::shared_ptr<Animation> animation = std::make_shared<Animation>(texture, width, height);

		for (auto sprite : sprites)
		{
			animation->addSprite(sprite);
		}

		animations.insert(std::pair<std::string, std::shared_ptr<Animation>>(index, animation));
	}

	std::shared_ptr<Animation> SpriteSheet::getAnimation(const std::string& index)
	{
		auto animation = animations.find(index);
		if (animation == animations.end())
			return nullptr;

		return animation->second;
	}
}