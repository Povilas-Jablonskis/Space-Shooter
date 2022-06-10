#include "Entity.hpp"
#include "InputComponent.hpp"
#include "Player.hpp"

#include <algorithm>

Entity::Entity(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color) : BaseGameObject(position, velocity, color)
{
	shootingModeFunc = []()
	{

	};
}

void Entity::addBullet(const std::shared_ptr<BaseGameObject>& bullet)
{
	bullet->onCollisionFunc = [=](const std::shared_ptr<BaseGameObject>& collider)
	{
		auto player = dynamic_cast<Player*>(collider.get());

		bullet->setNeedsToBeRemoved(true);

		if (player != nullptr)
		{
			if (player->getAddon("shield") != nullptr)
			{
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
		else
		{
			collider->setLives(collider->getLives() - 1);

			if (collider->getLives() < 1)
			{
				setValue(getValue() + collider->getValue());

				collider->setNeedsToBeRemoved(true);
			}
		}
	};
	getBulletsList()->push_back(bullet);
}

bool Entity::update(const float dt)
{
	BaseGameObject::update(dt);

	setDelayBetweenShootsTimer(getDelayBetweenShootsTimer() + dt);
	if (getDelayBetweenShootsTimer() > getDelayBetweenShoots())
	{
		setDelayBetweenShootsTimer(0.0f);
		shootingModeFunc();
	}

	auto bullets = getBulletsList();
	auto addons = getAddons();

	for (auto it = bullets->begin(); it != bullets->end();)
	{
		if ((*it)->update(dt))
		{
			it = bullets->erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = addons->begin(); it != addons->end();)
	{
		if ((*it).second->update(dt))
		{
			it = addons->erase(it);
		}
		else
		{
			++it;
		}
	}

	return getNeedsToBeRemoved();
}

std::shared_ptr<BaseGameObject> Entity::getAddon(const std::string& index)
{
	try
	{
		return m_addons.at(index);
	}
	catch (const std::exception&)
	{
		return nullptr;
	};
}

void Entity::addAddon(const std::string& key, const std::shared_ptr<BaseGameObject>& value)
{
	m_addons.insert_or_assign(key, value);
}