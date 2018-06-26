#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::~Player()
	{
		clearBullets();
	}

	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity)
	{
		setShootingSound("Sounds/sfx_laser2.ogg");
	}

	bool Player::update(float dt)
	{
		if (needsToBeRemoved)
		{
			needsToBeRemoved = false;
			respawn();
		}

		BaseGameObject::updateAnimation(dt);

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
			if ((*it).second->update(dt, position))
				it = addons->erase(it);
			else
				++it;
		}

		setShootingPosition(glm::vec2(position.x + (width / 2.0f), position.y + height));

		return true;
	}

	void Player::addBullet(std::shared_ptr<BaseGameObject> bullet, glm::vec2 offset)
	{
		bullet->setPosition(glm::vec2(position.x + (width / 2.0f) - (bullet->getSize(0) / 2.0f), position.y + height));
		bullet->setPosition(bullet->getPosition() + offset);
		bullet->setVelocity(1, 200.0f);
		bullet->onCollision = [this, bullet](std::shared_ptr<BaseGameObject> collider)
		{
			bullet->setNeedsToBeRemoved(true);

			auto params = std::map<std::string, BaseGameObject*>();
			params["bullet"] = bullet.get();
			params["parent"] = this;
			params["collider"] = collider.get();
			notify(ObserverEvent::BULLETDESTROYED, params);

			auto entity = dynamic_cast<Entity*>(collider.get());
			setScore(getScore() + collider->getValue());
			if (entity != nullptr && !entity->getNeedsToBeRemoved())
			{
				if (entity->getAddon("shield") != nullptr)
					entity->getAddon("shield")->setNeedsToBeRemoved(true);
				else
					entity->setNeedsToBeRemoved(true);
			}
			else
				collider->setNeedsToBeRemoved(true);
		};
		Entity::addBullet(bullet, offset);
	}

	void Player::respawn()
	{
		setHealth(getHealth()-1);
		if (getHealth() < 1)
			onDeath();
		else
			notify(ObserverEvent::PLAYERDIED, std::map<std::string, BaseGameObject*>());
		setVelocity(startVelocity);
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_WIDTH) / 2.0f, 0.0f));
	}
}