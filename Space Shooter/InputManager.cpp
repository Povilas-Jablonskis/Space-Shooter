#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager() : currentEditedKeyBinding(std::pair<std::string, std::shared_ptr<Text>>("", nullptr))
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

	bool InputManager::resetCurrentEditedKeyBinding()
	{
		auto currentKeyBinding = getCurrentEditedKeyBinding();
		if (keybindings.find(currentKeyBinding->first) != keybindings.end())
		{
			currentKeyBinding->second->setIsStatic(false);
			currentKeyBinding->second->changeColor(glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
			currentKeyBinding->first = "";
			currentKeyBinding->second.reset();
			return true;
		}
		return false;
	}

	void InputManager::fixInput()
	{
		for (int i = 0; i < 255; i++)
		{
			if (!GetAsyncKeyState(i) && pressedkeys[i])
				pressedkeys[i] = false;
		}
	}
}