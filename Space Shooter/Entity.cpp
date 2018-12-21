#include "Entity.h"
#include "Application.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShoots(0.0f), delayBetweenShootsTimer(0.0f) {
		setScale(0.5f);
		shootingMode = []()
		{

		};
	}

	void Entity::addBullet(std::shared_ptr<BaseGameObject> _bullet)
	{
		_bullet->onCollision = [_bullet, this](std::shared_ptr<BaseGameObject> collider)
		{
			auto entity = dynamic_cast<Entity*>(collider.get());

			_bullet->setNeedsToBeRemoved(true);

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
		bullets.push_back(_bullet);
	}

	bool Entity::update(float dt)
	{
		BaseGameObject::update(dt);

		setDelayBetweenShootsTimer(getDelayBetweenShootsTimer() + dt);
		if (getDelayBetweenShootsTimer() > getDelayBetweenShoots())
		{
			setDelayBetweenShootsTimer(0.0f);
			shootingMode();
		}

		auto bullets = getBulletsList();
		auto addons = getAddons();

		for (auto it = bullets->begin(); it != bullets->end();)
		{
			if ((*it)->update(dt))
				it = bullets->erase(it);
			else
				++it;
		}

		for (auto it = addons->begin(); it != addons->end();)
		{
			if ((*it).second->update(dt))
				it = addons->erase(it);
			else
				++it;
		}

		return getNeedsToBeRemoved();
	}

	std::shared_ptr<BaseGameObject> Entity::getAddon(std::string index)
	{
		for (auto it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
				return it->second;
		}
		return nullptr;
	}

	void Entity::addAddon(addon _addon)
	{
		for (auto it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == _addon.first)
				return;
		}

		addons.push_back(_addon);
	}
}