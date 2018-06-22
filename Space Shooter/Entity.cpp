#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), shootingSound("Sounds/sfx_laser1.ogg"), delayBetweenShoots(0.0f), delayBetweenShootsTimer(0.0f), shootingPosition(glm::vec2(0.0f, 0.0f))
	{
		shootingMode = [](Entity* entity)
		{

		};
	}

	void Entity::addBullet(std::shared_ptr<BaseGameObject> bullet, glm::vec2 offset)
	{ 
		auto params = std::map<std::string, BaseGameObject*>();
		params["collider"] = this;
		notify(ObserverEvent::BULLETSHOT, params);

		bullets.push_back(std::move(bullet));
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

	void Entity::addAddon(addon _addon)
	{
		for (auto addon : addons)
		{
			if (addon.first == _addon.first) return;
		}

		_addon.second->setPosition(getPosition() + _addon.second->getPositionOffset());
		addons.push_back(std::move(_addon));
	}
}