#include "ControlsMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"
#include "KeyBinding.hpp"
#include "SharedContext.hpp"
#include "FileConstants.hpp"

#include "rapidxml/RapidXMLSTD.hpp"
#include <fstream>
#include <algorithm>

ControlsMenu::ControlsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
    : m_sceneStateMachine(sceneStateMachine), m_context(context)
{

}

void ControlsMenu::onCreate()
{
	//Controls
	auto i = 0.0f;
	auto doc = new rapidxml::xml_document<>();
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::keybindingsSettingsPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(&buffer[0]);

	for (auto brewery_node = doc->first_node("KeyBindings"); brewery_node; brewery_node = brewery_node->next_sibling("KeyBindings"))
	{
		for (auto beer_node = brewery_node->first_node("KeyBinding"); beer_node; beer_node = beer_node->next_sibling("KeyBinding"))
		{
			std::string key = beer_node->first_attribute("key")->value();
			auto value = std::stoi(beer_node->first_attribute("value")->value());

			auto keybindingKey = std::make_shared<Text>(key + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f - 5.0f * i), *m_context.m_font);
			keybindingKey->disable();
			m_texts.push_back(keybindingKey);
			auto keybindingValue = std::make_shared<Text>(virtualKeyCodeToString(value), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(35.0f, 60.0f - 5.0f * i), *m_context.m_font);
			auto keybinding = std::make_shared<KeyBinding>(key, value, keybindingValue);
			keybindingValue->onMouseReleaseFunc = [=]()
			{
				if (m_context.m_inputManager->getCurrentlyEditedKeyBinding() == nullptr)
				{
					m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

					keybinding->setCurrentlyEdited(true);

					keybindingValue->disable();
					keybindingValue->onHoverEnterFuncDefaults();
				}
			};
			m_texts.push_back(keybindingValue);

			m_context.m_inputManager->addKeyBinding(keybinding);

			i += 1.0f;
		}
	}

	auto option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f), *m_context.m_font);
	option->onMouseReleaseFunc = [=]()
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
	};
	m_texts.push_back(option);

	theFile.close();
	doc->clear();
	delete doc;
}

void ControlsMenu::onActivate()
{
	m_context.m_inputManager->clearEverything();
}

void ControlsMenu::onDestroy() { }

void ControlsMenu::processInput()
{
	auto currentlyEditedKeyBinding = m_context.m_inputManager->getCurrentlyEditedKeyBinding();

	if (currentlyEditedKeyBinding == nullptr)
	{
		if (m_context.m_inputManager->getKey(27)) 
		{
			m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

			m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
		}
	}
	else 
	{
		if (m_context.m_inputManager->getKey(27)) // escape
		{
			currentlyEditedKeyBinding->getText()->enable();
			currentlyEditedKeyBinding->getText()->onHoverExitFuncDefaults();

			currentlyEditedKeyBinding->setCurrentlyEdited(false);
			m_context.m_inputManager->clearEverything();
			return;
		}

		auto& keys = m_context.m_inputManager->getKeys();
		auto& keyBindings = m_context.m_inputManager->getKeyBindings();

		for (auto& key : keys)
		{
			if (m_context.m_inputManager->getKey(key.first) && key.second)
			{
				if (key.first >= 32 && key.first < 127 && !std::any_of(keyBindings.begin(), keyBindings.end(), [=](const std::shared_ptr<KeyBinding>& pair) {return pair->getKeyBindingCharacter() == key.first; }))
				{
					currentlyEditedKeyBinding->getText()->enable();
					currentlyEditedKeyBinding->getText()->onHoverExitFuncDefaults();
					currentlyEditedKeyBinding->getText()->setText(virtualKeyCodeToString(key.first));

					currentlyEditedKeyBinding->setKeyBindingCharacter(key.first);
					currentlyEditedKeyBinding->setCurrentlyEdited(false);

					savePlayerConfig();
					m_context.m_inputManager->clearEverything();
					break;
				}
			}
		}
	}
}

void ControlsMenu::draw(float dt)
{
	auto& keybindings = m_context.m_inputManager->getKeyBindings();

	for (auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);
}

void ControlsMenu::savePlayerConfig() const
{
	auto& keyBindings = m_context.m_inputManager->getKeyBindings();
	auto doc = new rapidxml::xml_document<>();

	auto KeyBindings = doc->allocate_node(rapidxml::node_type::node_element, "KeyBindings");

	for (auto& keyBinding : keyBindings)
	{
		auto KeyBinding = doc->allocate_node(rapidxml::node_type::node_element, "KeyBinding");
		auto attribute_value = doc->allocate_string(keyBinding->getKeyBinding().c_str());
		KeyBinding->append_attribute(doc->allocate_attribute("key", attribute_value));
		attribute_value = doc->allocate_string(std::to_string(keyBinding->getKeyBindingCharacter()).c_str());
		KeyBinding->append_attribute(doc->allocate_attribute("value", attribute_value));
		KeyBindings->append_node(KeyBinding);
	}

	doc->append_node(KeyBindings);

	std::ofstream file_stored(FileConstants::keybindingsSettingsPath);
	file_stored << *doc;
	file_stored.close();
	doc->clear();
	delete doc;
}


std::string ControlsMenu::virtualKeyCodeToString(int virtualKey)
{
	auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	TCHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL: case VK_RMENU:
	case VK_LWIN: case VK_RWIN: case VK_APPS:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
		break;
	default:
		result = GetKeyNameText(scanCode << 16, szName, 128);
		break;
	}
	if (result == 0)
		throw std::system_error(std::error_code(GetLastError(), std::system_category()),
			"WinAPI Error occured.");
	return szName;
}