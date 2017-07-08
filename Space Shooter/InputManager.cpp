#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		SetLeftMouseState(false);
		SetLastLeftMouseState(false);
		SetRightMouseState(false);
		SetLastRightMouseState(false);
		for (int i = 0; i < 255; i++)
		{
			SetKey(i, false);
		}
	}

	InputManager::~InputManager()
	{

	}

	bool InputManager::GetLeftMouseState() const
	{
		return leftMouseClick;
	}

	bool InputManager::GetRightMouseState() const
	{
		return rightMouseClick;
	}

	void InputManager::SetLeftMouseState(bool boolean)
	{
		leftMouseClick = boolean;
	}

	void InputManager::SetRightMouseState(bool boolean)
	{
		rightMouseClick = boolean;
	}

	bool InputManager::GetKey(char key) const
	{
		return pressedkeys[key];
	}

	bool InputManager::GetKey(int key) const
	{
		return pressedkeys[key];
	}

	void InputManager::SetKey(char key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	void InputManager::SetKey(int key, bool boolean)
	{
		pressedkeys[key] = boolean;
	}

	bool InputManager::GetLastLeftMouseState() const
	{
		return lastLeftMouseClick;
	}

	bool InputManager::GetLastRightMouseState() const
	{
		return lastRightMouseClick;
	}

	void InputManager::SetLastLeftMouseState(bool boolean)
	{
		lastLeftMouseClick = boolean;
	}

	void InputManager::SetLastRightMouseState(bool boolean)
	{
		lastRightMouseClick = boolean;
	}

	void InputManager::SetLastMousePosition(glm::vec2 position)
	{
		lastMousePosition = position;
	}

	glm::vec2 InputManager::GetLastMousePosition() const
	{
		return lastMousePosition;
	}
}