#include "RenderObject.h"


namespace Engine
{
	RenderObject::RenderObject(int _width, int _height, glm::vec2 _position, glm::vec4 _color)
		: width(_width), height(_height), position(_position), color(_color), texture(nullptr), rotationAngle(0.0f), rotationAxis(glm::vec3(0.0,0.0,1.0))
	{

	}

	void RenderObject::updateTexture(float dt)
	{
		if (texture == nullptr) return;

		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			texture->setAnimTimer(texture->getAnimTimer() + dt);
			if (texture->getAnimTimer() > texture->getDelay())
			{
				texture->setAnimTimer(texture->getAnimTimer() - texture->getDelay());
				texture->setCurrentFrame(texture->getCurrentFrame() + 1);
				if (texture->getCurrentFrame() < texture->getStartFrame() || texture->getCurrentFrame() > texture->getEndFrame())
				{
					if (texture->getLoopStatus())
						texture->setCurrentFrame(texture->getStartFrame());
					else
					{
						texture->setCurrentFrame(texture->getEndFrame());
						texture->setAnimationStatus(true);
					}
				}
			}
		}
	}

	void RenderObject::applyTexture(std::shared_ptr<Texture> _texture)
	{
		if (_texture == nullptr) return;

		_texture->setAnimTimer(0.0f);
		texture = _texture;
	}

	int RenderObject::getSize(int index) const
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