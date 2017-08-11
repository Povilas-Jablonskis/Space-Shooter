#include "Player.h"
#include <algorithm>
#include <functional>
#include <string>

namespace Engine
{
	Player::~Player()
	{
		bullets.clear();
	}

	Player::Player(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), startHealth(3), health(startHealth), score(0), startVelocity(_velocity), lastHealth(0), lastScore(0), delayBetweenShoots(0.2f), delayBetweenShootsTimer(0.0f)
	{
		
	}

	std::shared_ptr<Addon> Player::getAddon(std::string index)
	{
		auto addon = addons.find(index);
		if (addon != addons.end())
			return addon->second;
		return nullptr;
	}

	void Player::removeAddon(std::string index)
	{
		auto addon = addons.find(index);
		if (addon != addons.end())
			addons.erase(addon);
	}

	bool Player::update(float dt, std::shared_ptr<InputManager> inputManager)
	{
		BaseGameObject::updateAnimation(dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		auto keyBindings = inputManager->getKeyBindings();

		delayBetweenShootsTimer += dt;

		if (delayBetweenShootsTimer > delayBetweenShoots && inputManager->getKey(keyBindings->find("Attack")->second))
		{
			delayBetweenShootsTimer = 0.0f;
			auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(position.x + (width / 2.0f), position.y + height + 5.0f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
			bullet->applyAnimation(animations["shoot"]);
			bullets.push_back(std::move(bullet));
		}

		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			if ((*it)->update(dt))
				it = bullets.erase(it);
			else
				++it;
		}

		for (std::map<std::string, std::shared_ptr<Addon>>::iterator it = addons.begin(); it != addons.end();)
		{
			if ((*it).second->update(dt, position))
				it = addons.erase(it);
			else
				++it;
		}

		if (inputManager->getKey(keyBindings->find("Move_Left")->second))
			position.x -= velocity.x * dt;
		if (inputManager->getKey(keyBindings->find("Move_Right")->second))
 			position.x += velocity.x * dt;
		if (inputManager->getKey(keyBindings->find("Move_Back")->second))
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
		if (getHealth() < 1)
			onDeath();
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
				(*it)->setNeedsToBeDeleted(true);
				return;
			}
		}
	}

	void Player::onCollision(BaseGameObject* collider)
	{
		#if _DEBUG
			std::cout << "player hit" << std::endl;
		#endif
	}
}