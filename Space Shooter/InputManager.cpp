#include "InputManager.h"
#include <iostream>

namespace Engine
{
	InputManager::InputManager() : leftMouseClick(false), lastLeftMouseClick(false), rightMouseClick(false), lastRightMouseClick(false), lastMousePosition(glm::vec2(0.0f, 0.0f)), currentlyEditedKeyBinding("")
	{

	}

	void InputManager::update()
	{
		if (getLeftMouseState() != getLastLeftMouseState())
		{
			setLastLeftMouseState(getLeftMouseState());
		}

		if (getRightMouseState() != getLastRightMouseState())
		{
			setLastRightMouseState(getRightMouseState());
		}
	}

	void InputManager::setKeyBinding(std::pair<std::string, int> pair)
	{
		for (auto it = keyBindings.begin(); it != keyBindings.end(); it++)
		{
			if ((*it).first == pair.first)
			{
				keyBindings.erase(it);
				break;
			}
		}

		keyBindings.insert(pair);
	}
}