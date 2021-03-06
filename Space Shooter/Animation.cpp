#include "Animation.hpp"

#include <Simple OpenGL Image Library/SOIL.h>

namespace Engine
{
	Animation::Animation(const GLuint spriteSheetTexture, const int width, const int height) : m_spriteSheetWidth(width), m_spriteSheetHeight(height), m_spriteSheetTexture(spriteSheetTexture)
	{

	}

	void Animation::loadFromFile(const std::string& path)
	{
		glGenTextures(1, &m_spriteSheetTexture);

		glBindTexture(GL_TEXTURE_2D, getSpriteSheetTexture()); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		int width, height;
		const auto image = SOIL_load_image(path.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentally mess up our texture.

		m_sprites.clear();
		setSpriteSheetWidth(width);
		setSpriteSheetHeight(height);
		m_sprites.emplace_back(width, height, width, height);
	}
}
