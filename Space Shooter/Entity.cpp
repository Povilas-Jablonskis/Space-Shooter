#include "Entity.hpp"
#include "InputComponent.hpp"

#include <algorithm>

namespace Engine
{
	Entity::Entity(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color) : BaseGameObject(position, velocity, color)
	{
		shootingModeFunc = []()
		{

		};
	}

	void Entity::addBullet(const std::shared_ptr<BaseGameObject>& bullet)
	{
		bullet->onCollisionFunc = [bullet, this](const std::shared_ptr<BaseGameObject>& collider)
		{
			auto entity = dynamic_cast<Entity*>(collider.get());

			bullet->setNeedsToBeRemoved(true);

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
		getBulletsList()->push_back(bullet);
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
		auto addons = getAddons();
		auto it = std::find_if(addons->begin(), addons->end(), [index](auto idx) { return idx.first == index; });

		return it != addons->end() ? it->second : nullptr;
	}

	void Entity::addAddon(addon addon)
	{
		auto addons = getAddons();

		for (auto it = addons->begin(); it != addons->end(); ++it)
		{
			if (it->first == addon.first)
			{
				addons->erase(it);
				break;
			}
		}

		addons->push_back(addon);
	}
}