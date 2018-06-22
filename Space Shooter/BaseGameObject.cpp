#include "BaseGameObject.h"
#include "Player.h"
#include "Enemy.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), needsToBeRemoved(false)
	{
		onDeath = []()
		{

		};

		onCollision = [this](std::shared_ptr<BaseGameObject> collider)
		{
			
		};
	}

	BaseGameObject::~BaseGameObject()
	{
		animations.clear();
	}

	bool BaseGameObject::update(float _dt)
	{
		if (getNeedsToBeRemoved()) onDeath();

		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateAnimation(_dt);
		return getNeedsToBeRemoved();
	}

	void BaseGameObject::addAnimation(std::string index, std::shared_ptr<Animation> _animation)
	{
		for (auto it = animations.begin(); it != animations.end(); it++)
		{
			if (it->first == index)
				return;
		}

		animations.push_back(std::move(animation(index, _animation)));
	}

	std::shared_ptr<Animation> BaseGameObject::getAnimationByIndex(std::string index)
	{
		for (auto animation : animations)
		{
			if (animation.first == index)
				return animation.second;
		}
		return nullptr;
	}
}