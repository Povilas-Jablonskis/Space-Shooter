#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		pressedkeys = new bool[pressedKeyCount];
		resetInput();
		theCurrentEditedKeyBinding = currentEditedKeyBinding(keyBindings.end(), nullptr);
	}

	void InputManager::resetInput()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		for (auto i = 0; i < pressedKeyCount; i++)
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
		for (auto i = 0; i < pressedKeyCount; i++)
		{
			if (!GetAsyncKeyState(i) && pressedkeys[i])
				pressedkeys[i] = false;
		}
	}

	void InputManager::setKeyBinding(std::string key, int value)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return;
		}

		keyBindings.push_back(keybinding(key, value));
	}

	int InputManager::getKeyBinding(std::string key)
	{
		for (auto keyBinding : keyBindings)
		{
			if (keyBinding.first == key)
				return keyBinding.second;
		}
		
		return -1;
	}

	void InputManager::updatePlayerInput(std::shared_ptr<Player> player, float dt)
	{
		player->setDelayBetweenShootsTimer(player->getDelayBetweenShootsTimer() + dt);
		if (player->getDelayBetweenShootsTimer() > player->getDelayBetweenShoots() && getKey(getKeyBinding("Attack")))
		{
			player->setDelayBetweenShootsTimer(0.0f);
			player->shootingMode();
		}

		if (getKey(getKeyBinding("Move Left")) || getKey(getKeyBinding("Move Right")) || getKey(getKeyBinding("Move Back")))
		{
			player->getAddon("leftExhaust")->applyAnimation(player->getAnimationByIndex("movingExhaust"));
			player->getAddon("rightExhaust")->applyAnimation(player->getAnimationByIndex("movingExhaust"));
		}
		
		if (!getKey(getKeyBinding("Move Left")) && !getKey(getKeyBinding("Move Right")) && !getKey(getKeyBinding("Move Back")))
		{
			player->getAddon("leftExhaust")->applyAnimation(player->getAnimationByIndex("noMovingExhaust"));
			player->getAddon("rightExhaust")->applyAnimation(player->getAnimationByIndex("noMovingExhaust"));
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