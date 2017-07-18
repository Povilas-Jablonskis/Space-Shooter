#include "Texture.h"
#include "SOIL.h"

namespace Engine
{
	Texture::Texture(int _startFame, int _endFrame, glm::vec2 _animsc) : startFrame(_startFame), endFrame(_endFrame), animsc(_animsc), width(0), height(0), texture(0)
	{

	}

	Texture::~Texture()
	{
	
	}

	void Texture::readTextureFile(const std::string& _path)
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
	}

	GLuint Texture::getTexture() const
	{
		return texture;
	}

	int Texture::getSize(int index) const
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
			default:
			{
				return NULL;
			}
		}
	}

	void Texture::setCount(glm::vec2 count)
	{
		animsc = count;
	}

	glm::vec2 Texture::getCount() const
	{
		return animsc;
	}

	int Texture::getStartFrame() const
	{
		return startFrame;
	}

	int Texture::getEndFrame() const
	{
		return endFrame;
	}

	void Texture::setFrames(int _startFrame, int _endFrame)
	{
		startFrame = _startFrame;
		endFrame = _endFrame;
	}
}