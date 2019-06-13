#include "BaseGameObject.h"
#include "Entity.h"
#include <string>

namespace Engine
{
	BaseGameObject::BaseGameObject(glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(0.0f, 0.0f, _position, _color), lives(1), velocity(_velocity), explosionSound(""), needsToBeRemoved(false), value(0)
	{
		onUpdateFunc = []()
		{

		};

		onCollisionFunc = [this](std::shared_ptr<BaseGameObject> collider)
		{
			auto entity = dynamic_cast<Entity*>(collider.get());

			setNeedsToBeRemoved(true);

			if (entity != nullptr)
			{
				if (entity->getAddon("shield") != nullptr)
				{
					entity->getAddon("shield")->setNeedsToBeRemoved(true);
				}
				else
				{
					entity->setNeedsToBeRemoved(true);
				}
			}
			else
			{
				collider->setNeedsToBeRemoved(true);
			}
		};
	}

	bool BaseGameObject::update(float _dt)
	{
		onUpdateFunc();

		if (getNeedsToBeRemoved())
		{
			notify(ObserverEvent::OBJECT_DESTROYED, this);

			setLives(getLives() - 1);
			if (getLives() > 0)
			{
				setNeedsToBeRemoved(false);
			}
		}

		setPosition(getPosition() + (velocity * _dt));

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

		animations.push_back(animation(index, _animation));
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