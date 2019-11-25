#include "KeyBindingInputComponent.hpp"
#include "Text.hpp"
#include "InputManager.hpp"

namespace Engine
{
	KeyBindingInputComponent::KeyBindingInputComponent(const std::string& kb) : m_keyBinding(kb)
	{

	}

	void KeyBindingInputComponent::update(Text* uiElement, const std::unique_ptr<InputManager>& inputManager)
	{
		if (inputManager->getCurrentlyEditedKeyBinding() != getKeyBinding())
		{
			return;
		}

		if (!inputManager->getLastKey(27) && inputManager->getKey(27)) // escape
		{
			uiElement->enable();
			uiElement->onHoverExitFuncDefaults();

			inputManager->setCurrentlyEditedKeyBinding("");
			return;
		}

		auto keys = *inputManager->getKeys();
		auto keyBindings = inputManager->getKeyBindings();

		for (auto key : keys)
		{
			if (!inputManager->getLastKey(key.first) && key.second)
			{
				if (key.first >= 32 && key.first < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [key](auto pair) {return pair.second == key.first; }))
				{
					uiElement->enable();
					uiElement->onHoverExitFuncDefaults();
					uiElement->setText(InputManager::virtualKeyCodeToString(key.first));

					inputManager->setKeyBinding(keyBinding(getKeyBinding(), key.first));
					inputManager->setCurrentlyEditedKeyBinding("");
					break;
				}
			}
		}
	}
}