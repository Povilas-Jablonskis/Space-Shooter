#include "SpriteSheet.hpp"

#include <stb/stb_image.h>
#include <fstream>

#include "rapidxml/rapidxml_ext.hpp"

SpriteSheet::SpriteSheet(const std::string& path)
{
	load(path);
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

void SpriteSheet::generateTexture(const std::string& path)
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

bool SpriteSheet::load(const std::string& path)
{
	const auto spriteSheetFileDoc = new rapidxml::xml_document<>();
	// Read the xml file into a vector
	std::ifstream spriteSheetFile(path);
	std::vector spriteSheetFileBuffer((std::istreambuf_iterator(spriteSheetFile)), std::istreambuf_iterator<char>());
	spriteSheetFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	spriteSheetFileDoc->parse<0>(spriteSheetFileBuffer.data());
	// Find our root node
	const rapidxml::xml_node<>* atlasNode = spriteSheetFileDoc->first_node();
	generateTexture(atlasNode->first_attribute("imagePath")->value());
	// Iterate over the breweries
	for (auto subTextureNode = atlasNode->first_node(); subTextureNode; subTextureNode = subTextureNode->next_sibling())
	{
		char* errPtr = nullptr;
		glm::vec4 animation;
		animation = glm::vec4(strtod(subTextureNode->first_attribute("x")->value(), &errPtr),
		                      strtod(subTextureNode->first_attribute("y")->value(), &errPtr),
		                      strtod(subTextureNode->first_attribute("width")->value(), &errPtr),
		                      strtod(subTextureNode->first_attribute("height")->value(), &errPtr));
		m_sprites.insert_or_assign(subTextureNode->first_attribute("name")->value(), animation);
	}

	spriteSheetFile.close();
	spriteSheetFileDoc->clear();
	delete spriteSheetFileDoc;
	return true;
}
