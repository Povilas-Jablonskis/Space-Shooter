#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::~Player()
	{

	}

	Player::Player(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity), lastHealth(0), lastScore(0), delayBetweenShoots(0.2f), delayBetweenShootsTimer(0.0f)
	{

	}

	bool Player::update(float dt, std::shared_ptr<InputManager> inputManager)
	{
		BaseGameObject::updateTexture(dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		
		for (auto BaseGameObject : bullets)
		{
			BaseGameObject->update(dt);
		}

		delayBetweenShootsTimer += dt;
		if (delayBetweenShootsTimer > delayBetweenShoots)
		{
			delayBetweenShootsTimer = 0.0f;
			if (inputManager->getKey(32))
			{
				auto bullet = std::make_shared<Bullet>(9, 20, glm::vec2(position.x + (width / 2.0f), position.y + height + 5.0f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
				bullet->applyTexture(animations["shoot"]);
				bullets.push_back(bullet);
			}
		}

		if (inputManager->getKey('a'))
			position.x -= velocity.x * dt;
		if (inputManager->getKey('d'))
 			position.x += velocity.x * dt;
		if (inputManager->getKey('s'))
			position.y -= velocity.y * dt;

		position.y += (velocity.y * dt) / 2.0f;

		if (position.x + width >= windowWidth)
			position.x = windowWidth - width;
		else if (position.x <= 0.0f)
			position.x = 0.0f;

		if (position.y + height >= windowHeigth)
			position.y = windowHeigth - height;
		else if (position.y <= 0.0f)
			position.y = 0.0f;
		return true;
	}

	void Player::respawn()
	{
		bullets.clear();
		setHealth(getHealth()-1);
		setVelocity(startVelocity);
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f));
	}

	void Player::restart()
	{
		bullets.clear();
		setScore(0);
		setHealth(startHealth);
		setVelocity(startVelocity);
		setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f));
	}

	void Player::deleteBullet(Bullet* bullet)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); it++)
		{
			if (it->get() == bullet)
			{
				bullets.erase(it);
				return;
			}
		}
	}

	void Player::onCollision(BaseGameObject* collider)
	{
		std::cout << "player hit" << std::endl;
	}
}