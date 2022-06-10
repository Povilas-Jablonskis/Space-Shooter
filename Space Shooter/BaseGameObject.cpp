#include "BaseGameObject.hpp"
#include "Entity.hpp"
#include "Player.hpp"

#include <algorithm>

BaseGameObject::BaseGameObject(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color) : RenderObject(0.0f, 0.0f, position, color), m_velocity(velocity)
{
	onUpdateFunc = []()
	{

	};

	onCollisionFunc = [=](const std::shared_ptr<BaseGameObject>& collider)
	{
		auto player = dynamic_cast<Player*>(collider.get());

		setLives(getLives() - 1);

		if (getLives() < 1)
		{
			setNeedsToBeRemoved(true);
		}

		if (player != nullptr)
		{
			if (player->getAddon("shield") != nullptr)
			{
				if (getNeedsToBeRemoved())
				{
					player->setValue(player->getValue() + getValue());
				}

				player->getAddon("shield")->setNeedsToBeRemoved(true);
			}
			else
			{
				player->setLives(player->getLives() - 1);
				player->setVelocity(player->getStartVelocity());
				player->setPosition(player->getStartPosition());

				if (player->getLives() < 1)
				{
					player->setNeedsToBeRemoved(true);
					player->setLives(0);
				}
			}
		}
	};
}

bool BaseGameObject::update(const float dt)
{
	onUpdateFunc();

	setPosition(getPosition() + getVelocity() * dt);

	updateAnimation(dt);
	return getNeedsToBeRemoved();
}

void BaseGameObject::addAnimation(const std::string& index, const std::shared_ptr<Animation>& t_animation)
{
	m_animations.insert_or_assign(index, t_animation);
}

std::shared_ptr<Animation> BaseGameObject::getAnimationByIndex(const std::string& index)
{
	return m_animations.at(index);
}