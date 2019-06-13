#include "RenderObject.h"

namespace Engine
{
	RenderObject::RenderObject(float _width, float _height, glm::vec2 _position, glm::vec4 _color)
		: width(_width), height(_height), position(_position), color(_color), theAnimation(nullptr), animComplete(false), animTimer(0.0f), currentFrame(0), scale(0.5f), rotationAngle(0.0f), rotationAxis(glm::vec3(0.0, 0.0, 1.0))
	{
		
	}

	void RenderObject::updateAnimation(float dt)
	{
		if (theAnimation == nullptr) return;

		auto sprites = theAnimation->getAnimation();

		if (sprites->size() > 1)
		{
			animTimer += dt;
			if (animTimer > (1.0f / 60.f))
			{
				animTimer = 0;
				currentFrame++;
				if (currentFrame >= sprites->size())
				{
					currentFrame = (sprites->size() - 1);
					animComplete = true;
				}
			}

			setOriginalWidth(theAnimation->getAnimation()->at(getCurrentFrame()).z);
			setOriginalHeight(theAnimation->getAnimation()->at(getCurrentFrame()).w);

			setWidth(theAnimation->getAnimation()->at(getCurrentFrame()).z);
			setHeight(theAnimation->getAnimation()->at(getCurrentFrame()).w);
		}
	}

	void RenderObject::applyAnimation(std::shared_ptr<Animation> _animation)
	{
		if (_animation == nullptr || _animation == theAnimation) return;

		theAnimation = _animation;
		animTimer = 0.0;
		animComplete = false;
		currentFrame = 0;

		setOriginalWidth(theAnimation->getAnimation()->at(getCurrentFrame()).z);
		setOriginalHeight(theAnimation->getAnimation()->at(getCurrentFrame()).w);

		setWidth(theAnimation->getAnimation()->at(getCurrentFrame()).z);
		setHeight(theAnimation->getAnimation()->at(getCurrentFrame()).w);
	}
}