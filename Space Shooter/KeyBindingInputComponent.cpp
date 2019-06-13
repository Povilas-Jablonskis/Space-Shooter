#include "KeyBindingInputComponent.h"
#include "Text.h"
#include <algorithm>

namespace Engine
{
	KeyBindingInputComponent::KeyBindingInputComponent(std::string kb) : keyBinding(kb)
	{

	}

	void KeyBindingInputComponent::update(Text* uiElement, std::shared_ptr<InputManager> inputManager)
	{
		if (inputManager->getCurrentlyEditedKeyBinding() != keyBinding) return;

		if (!inputManager->getLastKey(27) && inputManager->getKey(27)) // escape
		{
			uiElement->enable();
			uiElement->onHoverExitFuncDefaults();

			inputManager->setCurrentlyEditedKeyBinding("");
			return;
		}

		auto keys = inputManager->getKeys();
		auto keyBindings = inputManager->getKeyBindings();

		for (auto key : keys)
		{
			if (!inputManager->getLastKey(key.first) && key.second)
			{
				if (key.first >= 32 && key.first < 127 && !std::any_of(keyBindings.begin(), keyBindings.end(), [key](std::pair<std::string, int> pair) {return pair.second == key.first; }))
				{
					uiElement->enable();
					uiElement->onHoverExitFuncDefaults();
					uiElement->setText(virtualKeyCodeToString(key.first));

					inputManager->setKeyBinding(std::pair<std::string, int>(keyBinding, key.first));
					inputManager->setCurrentlyEditedKeyBinding("");
					break;
				}
			}
		}
	}

	std::string KeyBindingInputComponent::virtualKeyCodeToString(int virtualKey)
	{
		auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		CHAR szName[128];
		auto result = 0;
		switch (virtualKey)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= 0x100;
			default:
				result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}
		return szName;
	}
}