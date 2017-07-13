#include "Texture.h"
#include "SOIL.h"

namespace Engine
{
	Texture::Texture() : startFrame(0), endFrame(0), currentFrame(0), animComplete(false), animTimer(0.0f), loop(false), animationDone(false), delay(1.0f), animsc(0), width(0), height(0), texture(0)
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

	void Texture::setCurrentFrame(int frame)
	{
		currentFrame = frame;
	}

	int Texture::getCurrentFrame() const
	{
		return currentFrame;
	}

	void Texture::setFrames(int _startFrame, int _endFrame)
	{
		startFrame = _startFrame;
		endFrame = _endFrame;
	}

	void Texture::setDelay(float _delay)
	{
		delay = _delay;
	}

	void Texture::setAnimationStatus(bool _status)
	{
		animationDone = _status;
	}

	void Texture::setLoopStatus(bool _status)
	{
		loop = _status;
	}

	void Texture::update(float dt)
	{
		if (endFrame - startFrame > 0)
		{
			animTimer += dt;
			if (animTimer > delay)
			{
				animTimer -= delay;
				currentFrame++;
				if (currentFrame < startFrame || currentFrame > endFrame)
				{
					if (loop == true)
						currentFrame = startFrame;
					else
					{
						currentFrame = endFrame;
						animComplete = true;
					}
				}
			}
		}
	}
}