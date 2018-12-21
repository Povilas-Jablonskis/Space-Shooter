#include "Pickup.h"

namespace Engine
{
	Pickup::Pickup(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), needsToBeRemoved(false)
	{
		onCollision = [](std::shared_ptr<BaseGameObject> collider)
		{

		};
	}

	Pickup::~Pickup()
	{
		animations.clear();
	}

	bool Pickup::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateAnimation(_dt);
		return getNeedsToBeRemoved();
	}

	void Pickup::applyAnimation(std::shared_ptr<Animation> _animation)
	{
		if (_animation == nullptr || _animation == theAnimation) return;

		RenderObject::applyAnimation(_animation);

		setWidth(theAnimation->getAnimation()->at(getCurrentFrame()).z);
		setHeight(theAnimation->getAnimation()->at(getCurrentFrame()).w);
	}

	void Pickup::addAnimation(std::string index, std::shared_ptr<Animation> _animation)
	{
		for (auto it = animations.begin(); it != animations.end(); it++)
		{
			if (it->first == index)
				return;
		}

		animations.push_back(animation(index, _animation));
	}

	std::shared_ptr<Animation> Pickup::getAnimationByIndex(std::string index)
	{
		for (auto animation : animations)
		{
			if (animation.first == index)
				return animation.second;
		}
		return nullptr;
	}
}