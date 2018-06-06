#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShootsTimer(0.0f)
	{
		setDelayBetweenShoots(0.25f);
		setShootingSound("Sounds/lasers/4.wav");
		shootingMode = [](Entity* entity)
		{

		};
	}

	void Entity::addBullet(std::shared_ptr<BaseGameObject> bullet)
	{ 
		bullet->onCollision = [bullet](std::shared_ptr<BaseGameObject> collider)
		{
			bullet->setNeedsToBeDeleted(true);

			auto entity = dynamic_cast<Entity*>(collider.get());
			if (entity != nullptr)
			{
				if (entity->getAddon("shield") != nullptr)
					entity->removeAddon("shield");
				else
					entity->setNeedsToBeDeleted(true);
			}
			else
				collider->setNeedsToBeDeleted(true);
		};
		bullets.push_back(bullet); 
	}

	std::shared_ptr<Addon> Entity::getAddon(std::string index)
	{
		for (auto it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
				return it->second;
		}
		return nullptr;
	}

	void Entity::removeAddon(std::string index)
	{
		for (auto it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
			{
				addons.erase(it);
				return;
			}
		}
	}

	void Entity::addAddon(std::pair<std::string, std::shared_ptr<Addon>> _addon)
	{
		for (auto addon : addons)
		{
			if (addon.first == _addon.first) return;
		}

		_addon.second->setPosition(getPosition() + _addon.second->getPositionOffset());
		addons.push_back(_addon);
	}
}