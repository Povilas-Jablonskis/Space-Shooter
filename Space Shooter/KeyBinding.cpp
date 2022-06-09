#include "KeyBinding.hpp"
#include "Text.hpp"
#include "InputManager.hpp"

#include <algorithm>
#include "rapidxml/RapidXMLSTD.hpp"
#include <fstream>
#include <utility>

KeyBinding::KeyBinding(std::string keyBinding, const short keyBindingCharacter) : m_keyBindingCharacter(keyBindingCharacter), m_keyBinding(std::move(keyBinding))
{

}

void KeyBinding::savePlayerConfig(const std::shared_ptr<InputManager>& inputManager) const
{
	const auto keyBindings = *inputManager->getKeyBindings();
	auto doc = new rapidxml::xml_document<>();

	auto KeyBindings = doc->allocate_node(rapidxml::node_type::node_element, "KeyBindings");

	for (const auto& keyBinding : keyBindings)
	{
		auto KeyBinding = doc->allocate_node(rapidxml::node_type::node_element, "KeyBinding");
		auto attribute_value = doc->allocate_string(keyBinding->getKeyBinding().c_str());
		KeyBinding->append_attribute(doc->allocate_attribute("key", attribute_value));
		attribute_value = doc->allocate_string(std::to_string(keyBinding->getKeyBindingCharacter()).c_str());
		KeyBinding->append_attribute(doc->allocate_attribute("value", attribute_value));
		KeyBindings->append_node(KeyBinding);
	}

	doc->append_node(KeyBindings);

	std::ofstream file_stored("cfg/keyBindingSettings.xml");
	file_stored << *doc;
	file_stored.close();
	doc->clear();
	delete doc;
}

void KeyBinding::update(const std::shared_ptr<InputManager>& inputManager)
{
	if (inputManager->getCurrentlyEditedKeyBinding() != nullptr && inputManager->getCurrentlyEditedKeyBinding()->getKeyBinding() != getKeyBinding())
	{
		return;
	}

	if (!inputManager->getLastKey(27) && inputManager->getKey(27)) // escape
	{
		getText()->enable();
		getText()->onHoverExitFuncDefaults();

		inputManager->setCurrentlyEditedKeyBinding(nullptr);
		return;
	}

	auto keys = *inputManager->getKeys();
	auto keyBindings = inputManager->getKeyBindings();

	for (auto key : keys)
	{
		if (!inputManager->getLastKey(key.first) && key.second)
		{
			if (key.first >= 32 && key.first < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [key](auto pair) {return pair->getKeyBindingCharacter() == key.first; }))
			{
				getText()->enable();
				getText()->onHoverExitFuncDefaults();
				getText()->setText(InputManager::virtualKeyCodeToString(key.first));

				setKeyBindingCharacter(key.first);
				inputManager->setCurrentlyEditedKeyBinding(nullptr);

				savePlayerConfig(inputManager);
				break;
			}
		}
	}
}