#include "SpriteSheet.hpp"

#include <stb/stb_image.h>
#include "rapidxml/rapidxml.hpp"

#include <fstream>

SpriteSheet::SpriteSheet(const std::string& path)
{
	loadSpritesFromXml(path);
}

const glm::vec4& SpriteSheet::getSprite(const std::string& index) const
{
	return m_sprites.at(index);
}

int SpriteSheet::getWidth() const
{
	return m_width;
}

int SpriteSheet::getHeight() const
{
	return m_height;
}

GLuint SpriteSheet::getTexture() const
{
	return m_texture;
}

void SpriteSheet::loadSpriteSheet(const std::string& path)
{
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT
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

bool SpriteSheet::loadSpritesFromXml(const std::string& path)
{
	const auto doc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream theFile(path);
	std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(buffer.data());
	// Find our root node
	const rapidxml::xml_node<>* root_node = doc->first_node();
	loadSpriteSheet(root_node->first_attribute("imagePath")->value());
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node(); brewery_node; brewery_node = brewery_node->next_sibling())
	{
		char* err_ptr;
		glm::vec4 animation;
		animation = glm::vec4(strtod(brewery_node->first_attribute("x")->value(), &err_ptr),
		                      strtod(brewery_node->first_attribute("y")->value(), &err_ptr),
		                      strtod(brewery_node->first_attribute("width")->value(), &err_ptr),
		                      strtod(brewery_node->first_attribute("height")->value(), &err_ptr));
		m_sprites.insert_or_assign(brewery_node->first_attribute("name")->value(), animation);
	}

	theFile.close();
	doc->clear();
	delete doc;
	return true;
}
