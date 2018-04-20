#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>
#include "Application.h"

namespace Engine
{
	Player::~Player()
	{
		clearBullets();
	}

	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity)
	{
		setDelayBetweenShoots(0.2f);
		setShootingType(ShootingType::NORMAL);
		setShootingSound("Sounds/lasers/7.wav");
	}

	bool Player::update(float dt)
	{
		if (needsToBeDeleted)
		{
			needsToBeDeleted = false;
			respawn();
			return false;
		}

		BaseGameObject::updateAnimation(dt);

		auto bullets = getBulletsList();
		auto addons = getAddons();

		for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = bullets->begin(); it != bullets->end();)
		{
			if ((*it)->update(dt))
				it = bullets->erase(it);
			else
				++it;
		}

		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons->begin(); it != addons->end();)
		{
			if ((*it).second->update(dt, position))
				it = addons->erase(it);
			else
				++it;
		}
		return true;
	}

	void Player::respawn()
	{
		clearBullets();
		setHealth(getHealth()-1);
		if (getHealth() < 1)
			onDeath();
		setVelocity(startVelocity);
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f));
	}

	void Player::restart()
	{
		setDelayBetweenShoots(0.25f);
		setShootingType(ShootingType::NORMAL);
		clearBullets();
		setScore(0);
		setHealth(startHealth);
		setVelocity(startVelocity);
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f));
	}
}