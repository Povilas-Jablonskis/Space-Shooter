#include "Entity.h"

namespace Engine
{
	Entity::Entity(glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_position, _velocity, _color), delayBetweenShoots(0.0f), delayBetweenShootsTimer(0.0f), inputComponent(nullptr)
	{
		shootingModeFunc = []()
		{

		};
	}

	Entity::Entity(glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, std::shared_ptr<InputComponent> input)
		: BaseGameObject(_position, _velocity, _color), delayBetweenShoots(0.0f), delayBetweenShootsTimer(0.0f), inputComponent(input)
	{
		shootingModeFunc = []()
		{

		};
	}

	void Entity::addBullet(std::shared_ptr<BaseGameObject> _bullet)
	{
		_bullet->onCollisionFunc = [_bullet, this](std::shared_ptr<BaseGameObject> collider)
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
			shootingModeFunc();
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
			{
				addons.erase(it);
				break;
			}
		}

		addons.push_back(_addon);
	}
}