#include "RenderObject.h"


namespace Engine
{
	RenderObject::RenderObject(float _width, float _height, glm::vec2 _position, glm::vec4 _color)
		: width(_width), height(_height), position(_position), color(_color), texture(nullptr), animComplete(false), animTimer(0.0f), currentFrame(0), rotationAngle(0.0f), rotationAxis(glm::vec3(0.0, 0.0, 1.0))
	{

	}

	void RenderObject::updateTexture(float dt)
	{
		if (texture == nullptr) return;

		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			animTimer += dt;
			if (animTimer > texture->getDelay())
			{
				animTimer = 0;
				currentFrame++;
				if (currentFrame < texture->getStartFrame() || currentFrame > texture->getEndFrame())
				{
					if (texture->getLoopStatus())
						currentFrame = texture->getStartFrame();
					else
					{
						currentFrame = texture->getEndFrame();
						animComplete = true;
					}
				}
			}
		}
	}

	void RenderObject::applyTexture(std::shared_ptr<Texture> _texture)
	{
		if (_texture == nullptr || _texture == texture) return;

		texture = _texture;
		animTimer = 0.0;
		animComplete = false;
		currentFrame = texture->getStartFrame();
	}		

	float RenderObject::getSize(int index) const
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
}