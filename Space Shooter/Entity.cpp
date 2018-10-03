#include "Entity.h"

namespace Engine
{
	Entity::Entity(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShoots(0.0f), delayBetweenShootsTimer(0.0f) {
		setScale(0.5f);
		shootingMode = []()
		{

		};
	}

	void Entity::addBullet(glm::vec2 bulletPosition, glm::vec2 velocity, std::string explosionSound, std::string shootingSound)
	{ 
		auto bullet = std::make_shared<BaseGameObject>(0.0f, 0.0f, bulletPosition, velocity, glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		bullet->applyAnimation(getAnimationByIndex("shoot"));
		bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
		bullet->setScale(0.5f);
		bullet->onUpdate = [bullet]()
		{
			auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
			auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

			//Collision detection
			if (bullet->getPosition(0) + bullet->getWidth() >= windowWidth || bullet->getPosition(0) <= 0.0f)
				bullet->setNeedsToBeRemoved(true);

			if (bullet->getPosition(1) + bullet->getHeight() >= windowHeigth || bullet->getPosition(1) <= 0.0f)
				bullet->setNeedsToBeRemoved(true);
		};
		bullet->onCollision = [bullet, explosionSound, this](std::shared_ptr<BaseGameObject> collider)
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
					auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
					params.push_back(std::pair<std::string, BaseGameObject*>(explosionSound, nullptr));
					params.push_back(std::pair<std::string, BaseGameObject*>("collider", collider.get()));
					notify(ObserverEvent::OBJECT_DESTROYED, params);

					entity->setNeedsToBeRemoved(true);
				}
			}
			else
			{
				auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
				params.push_back(std::pair<std::string, BaseGameObject*>(explosionSound, nullptr));
				params.push_back(std::pair<std::string, BaseGameObject*>("collider", collider.get()));
				notify(ObserverEvent::OBJECT_DESTROYED, params);

				collider->setNeedsToBeRemoved(true);
			}
		};
		bullets.push_back(bullet);

		auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
		params.push_back(std::pair<std::string, BaseGameObject*>(shootingSound, nullptr));
		notify(ObserverEvent::BULLET_SHOT, params);
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