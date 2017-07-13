#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		setLeftMouseState(false);
		setLastLeftMouseState(false);
		setRightMouseState(false);
		setLastRightMouseState(false);
		for (int i = 0; i < 255; i++)
		{
			setKey(i, false);
		}
	}

	InputManager::~InputManager()
	{

	}

	bool InputManager::getLeftMouseState() const
	{
		return leftMouseClick;
	}

	bool InputManager::getRightMouseState() const
	{
		return rightMouseClick;
	}

	void InputManager::setLeftMouseState(bool boolean)
	{
		leftMouseClick = boolean;
	}

	void InputManager::setRightMouseState(bool boolean)
	{
		rightMouseClick = boolean;
	}

	bool InputManager::getKey(char key) const
	{
		return pressedkeys[key];
	}

	bool InputManager::getKey(int key) const
	{
		return pressedkeys[key];
	}

	void InputManager::setKey(char key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	void InputManager::setKey(int key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	bool InputManager::getLastLeftMouseState() const
	{
		return lastLeftMouseClick;
	}

	bool InputManager::getLastRightMouseState() const
	{
		return lastRightMouseClick;
	}

	void InputManager::setLastLeftMouseState(bool boolean)
	{
		lastLeftMouseClick = boolean;
	}

	void InputManager::setLastRightMouseState(bool boolean)
	{
		lastRightMouseClick = boolean;
	}

	void InputManager::setLastMousePosition(glm::vec2 position)
	{
		lastMousePosition = position;
	}

	glm::vec2 InputManager::getLastMousePosition() const
	{
		return lastMousePosition;
	}
}