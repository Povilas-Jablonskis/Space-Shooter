#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		resetInput();
	}

	void InputManager::resetInput()
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
}