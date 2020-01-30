#include "KeyBindingInputComponent.hpp"
#include "Text.hpp"
#include "InputManager.hpp"

#include <algorithm>
#include "rapidxml/rapidxml_print.hpp"
#include <fstream>
#include <utility>

namespace Engine
{
	KeyBindingInputComponent::KeyBindingInputComponent(std::string kb) : m_keyBinding(std::move(kb))
	{

	}

	void KeyBindingInputComponent::savePlayerConfig(const std::shared_ptr<InputManager>& inputManager) const
	{
		const auto keyBindings = *inputManager->getKeyBindings();
		auto doc = new rapidxml::xml_document<>();

		auto KeyBindings = doc->allocate_node(rapidxml::node_element, "KeyBindings");

		for (const auto& keyBinding : keyBindings)
		{
			auto KeyBinding = doc->allocate_node(rapidxml::node_element, "KeyBinding");
			auto attribute_value = doc->allocate_string(keyBinding.first.c_str());
			KeyBinding->append_attribute(doc->allocate_attribute("key", attribute_value));
			attribute_value = doc->allocate_string(std::to_string(keyBinding.second).c_str());
			KeyBinding->append_attribute(doc->allocate_attribute("value", attribute_value));
			KeyBindings->append_node(KeyBinding);
		}

		doc->append_node(KeyBindings);

		std::ofstream file_stored("Config/keyBindingSettings.xml");
		file_stored << *doc;
		file_stored.close();
		doc->clear();
		delete doc;
	}
	
	void KeyBindingInputComponent::update(Text* uiElement, const std::shared_ptr<InputManager>& inputManager) const
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

					savePlayerConfig(inputManager);
					break;
				}
			}
		}
	}
}
