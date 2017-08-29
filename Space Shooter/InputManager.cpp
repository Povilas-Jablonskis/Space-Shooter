#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		pressedkeys = new bool[pressedKeyCount];
		resetInput();
		setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>(keyBindings.end(), nullptr));
	}

	void InputManager::resetInput()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		for (int i = 0; i < pressedKeyCount; i++)
		{
			setKey(i, false);
		}
	}

	bool InputManager::resetCurrentEditedKeyBinding()
	{
		auto currentKeyBinding = getCurrentEditedKeyBinding();

		if (currentKeyBinding->second == nullptr)
			return false;

		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == currentKeyBinding->first->first)
			{
				currentKeyBinding->second->setIsStatic(false);
				currentKeyBinding->second->changeColor(glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
				currentKeyBinding->second.reset();
				return true;
			}
		}

		return false;
	}

	void InputManager::fixInput()
	{
		for (int i = 0; i < pressedKeyCount; i++)
		{
			if (!GetAsyncKeyState(i) && pressedkeys[i])
				pressedkeys[i] = false;
		}
	}

	void InputManager::setKeyBinding(const std::string& key, int value)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return;
		}

		keyBindings.push_back(std::pair<std::string, int>(key, value));
	}

	int InputManager::getKeyBinding(const std::string& key)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return keyBinding.second;
		}
		
		return -1;
	}

	void InputManager::updatePlayerInput(Player* player, float dt)
	{
		player->setDelayBetweenShootsTimer(player->getDelayBetweenShootsTimer() + dt);
		if (player->getDelayBetweenShootsTimer() > player->getDelayBetweenShoots() && getKey(getKeyBinding("Attack")))
		{
			player->setDelayBetweenShootsTimer(0.0f);

			switch (player->getShootingType())
			{
				case NORMAL:
				{
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));
					break;
				}
				case DOUBLE:
				{
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));
					break;
				}
				case HALFCIRCLE:
				{
					//Middle
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 20.0f, player->getPosition(1) + player->getSize(1) + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 40.0f, player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 20.0f, player->getPosition(1) + player->getSize(1) + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 40.0f, player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));
					break;
				}
				case DOUBLEHALFCIRCLE:
				{
					////////////1

					//Middle
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 20.0f, player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 40.0f, player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 20.0f, player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 40.0f, player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					////////////2

					//Middle
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 20.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 20.0f, player->getPosition(1) + player->getSize(1) + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) - 40.0f, player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 20.0f, player->getPosition(1) + player->getSize(1) + 15.0f + 4.5f + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f) + 40.0f, player->getPosition(1) + player->getSize(1) + 4.5f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", player->getAnimationByIndex("explosion"));
					bullet->applyAnimation(player->getAnimationByIndex("shoot"));
					player->addBullet(std::move(bullet));
					break;
				}
			}
		}

		if (getKey(getKeyBinding("Move Left")))
			player->setPosition(0, player->getPosition(0) - (player->getVelocity(0) * dt));
		if (getKey(getKeyBinding("Move Right")))
			player->setPosition(0, player->getPosition(0) + (player->getVelocity(0) * dt));
		if (getKey(getKeyBinding("Move Back")))
			player->setPosition(1, player->getPosition(1) - (player->getVelocity(1) * dt));

		player->setPosition(1, player->getPosition(1) + ((player->getVelocity(1) * dt) / 2.0f));
	}
}