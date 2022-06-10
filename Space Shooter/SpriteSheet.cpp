#include "SpriteSheet.hpp"
#include "Animation.hpp"

#include <stb/stb_image.h>
#include "rapidxml/rapidxml.hpp"

#include <fstream>
#include <algorithm>

SpriteSheet::SpriteSheet(const std::string& spriteSheetName, const std::string& spriteSheetNameXml)
{
	loadSpriteSheet(spriteSheetName);
	loadSpritesFromXml(spriteSheetNameXml);
}

void SpriteSheet::loadSpriteSheet(const std::string& path)
{
	if (path.empty())
	{
		return;
	}

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	const auto image = stbi_load(path.c_str(), &m_width, &m_height, nullptr, STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentally mess up our texture.

	m_sprites.insert_or_assign("wholeSpriteSheet", glm::vec4(m_width, m_height, m_width, m_height));
}

void SpriteSheet::loadSpritesFromXml(const std::string& path)
{
	if (path.empty())
	{
		return;
	}

	auto doc = new rapidxml::xml_document<>();
	// Read the xml file into a vector
	std::ifstream theFile(path);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(&buffer[0]);
	// Find our root node
	rapidxml::xml_node<>* root_node = doc->first_node();
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node(); brewery_node; brewery_node = brewery_node->next_sibling())
	{
		auto animation = glm::vec4(atof(brewery_node->first_attribute("x")->value()),
			atof(brewery_node->first_attribute("y")->value()),
			atof(brewery_node->first_attribute("width")->value()),
			atof(brewery_node->first_attribute("height")->value()));
		m_sprites.insert_or_assign(brewery_node->first_attribute("name")->value(), animation);
	}

	theFile.close();
	doc->clear();
	delete doc;
}

std::shared_ptr<Animation> SpriteSheet::getSprite(const std::string& index)
{
	auto& sprite = m_sprites.at(index);

	auto animation = std::make_shared<Animation>(m_texture, getWidth(), getHeight());
	animation->addSprite(sprite);
	return animation;
}

glm::vec4 SpriteSheet::getSpriteAsVector(const std::string& index) const
{
	return m_sprites.at(index);
}

void SpriteSheet::makeAnimation(const std::string& index, const std::vector<std::string>& indexes)
{
	auto _animation = std::make_shared<Animation>(getTexture(), getWidth(), getHeight());

	for (const auto& _index : indexes)
	{
		for (const auto& sprite : m_sprites)
		{
			if (sprite.first == _index)
			{
				_animation->addSprite(sprite.second);
				break;
			}
		}
	}

	m_animations.insert_or_assign(index, _animation);
}

void SpriteSheet::makeAnimation(const std::string& index, const std::vector<glm::vec4>& sprites)
{
	auto _animation = std::make_shared<Animation>(getTexture(), getWidth(), getHeight());

	for (auto& sprite : sprites)
	{
		_animation->addSprite(sprite);
	}

	m_animations.insert_or_assign(index, _animation);
}

std::shared_ptr<Animation> SpriteSheet::getAnimation(const std::string& index) const
{
	return m_animations.at(index);
}
