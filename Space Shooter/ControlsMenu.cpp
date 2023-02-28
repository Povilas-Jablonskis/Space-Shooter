#include "ControlsMenu.hpp"
#include "InputManager.hpp"
#include "KeyBinding.hpp"
#include "FileConstants.hpp"

#include "rapidxml/RapidXMLSTD.hpp"
#include <fstream>
#include <algorithm>
#include <freeglut/freeglut.h>

ControlsMenu::ControlsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void ControlsMenu::onCreate()
{
	//Controls
	auto index = 0.0f;
	const auto keyBindingsFileDoc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream keyBindingsFile(FileConstants::KEYBINDINGS_SETTINGS_PATH);
	std::vector keyBindingsFileBuffer((std::istreambuf_iterator(keyBindingsFile)), std::istreambuf_iterator<char>());
	keyBindingsFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	keyBindingsFileDoc->parse<0>(keyBindingsFileBuffer.data());

	for (auto keyBindingsNode = keyBindingsFileDoc->first_node("KeyBindings"); keyBindingsNode; keyBindingsNode =
	     keyBindingsNode->
	     next_sibling("KeyBindings"))
	{
		for (auto keyBindingNode = keyBindingsNode->first_node("KeyBinding"); keyBindingNode; keyBindingNode =
		     keyBindingNode->
		     next_sibling("KeyBinding"))
		{
			std::string keyNode = keyBindingNode->first_attribute("key")->value();
			auto valueNode = std::stoi(keyBindingNode->first_attribute("value")->value());

			auto keyBindingKey = std::make_shared<Text>(keyNode + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
			                                            glm::vec2(20.0f, 60.0f - 5.0f * index), *m_context.font);
			keyBindingKey->disable();
			m_texts.push_back(keyBindingKey);
			auto keyBindingValue = std::make_shared<Text>(sVirtualKeyCodeToString(valueNode),
			                                              glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
			                                              glm::vec2(35.0f, 60.0f - 5.0f * index), *m_context.font);
			auto keyBinding = std::make_shared<KeyBinding>(keyNode, valueNode, keyBindingValue);
			keyBindingValue->onMouseReleaseFunc = [=, this]
			{
				if (m_context.inputManager->getCurrentlyEditedKeyBinding() == nullptr)
				{
					m_context.soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

					keyBinding->setCurrentlyEdited(true);

					keyBindingValue->disable();
					keyBindingValue->onHoverEnterFuncDefaults();
				}
			};
			m_texts.push_back(keyBindingValue);

			m_context.inputManager->addKeyBinding(keyBinding);

			index += 1.0f;
		}
	}

	const auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(48.0f, 20.0f),
	                                               *m_context.font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
	};
	m_texts.push_back(backOption);

	keyBindingsFile.close();
	keyBindingsFileDoc->clear();
	delete keyBindingsFileDoc;
}

void ControlsMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void ControlsMenu::onDestroy()
{
}

void ControlsMenu::processInput()
{
	const auto currentlyEditedKeyBinding = m_context.inputManager->getCurrentlyEditedKeyBinding();

	if (currentlyEditedKeyBinding == nullptr)
	{
		if (m_context.inputManager->isKeyActive(27))
		{
			m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

			m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
		}
	}
	else
	{
		if (m_context.inputManager->isKeyActive(27)) // escape
		{
			currentlyEditedKeyBinding->getText()->enable();
			currentlyEditedKeyBinding->getText()->onHoverExitFuncDefaults();

			currentlyEditedKeyBinding->setCurrentlyEdited(false);
			m_context.inputManager->clearEverything();
			return;
		}

		const auto& keys = m_context.inputManager->getKeys();
		auto& keyBindings = m_context.inputManager->getKeyBindings();

		for (const auto& [key, isActive] : keys)
		{
			if (m_context.inputManager->isKeyActive(key) && isActive)
			{
				if (key >= 32 && key < 127 && !std::ranges::any_of(keyBindings,
				                                                   [=](const std::shared_ptr<KeyBinding>&
				                                                   pair)
				                                                   {
					                                                   return pair->getKeyBindingCharacter()
						                                                   == key;
				                                                   }))
				{
					auto& currentlyEditedKeyBindingText = currentlyEditedKeyBinding->getText();
					currentlyEditedKeyBindingText->enable();
					currentlyEditedKeyBindingText->onHoverExitFuncDefaults();
					currentlyEditedKeyBindingText->setText(sVirtualKeyCodeToString(key));

					currentlyEditedKeyBinding->setKeyBindingCharacter(key);
					currentlyEditedKeyBinding->setCurrentlyEdited(false);

					savePlayerConfig();
					m_context.inputManager->clearEverything();
					break;
				}
			}
		}
	}
}

void ControlsMenu::draw(float)
{
	for (const auto& text : m_texts)
	{
		text->update();
	}

	for (const auto& text : m_texts)
	{
		m_context.inputManager->checkInteraction(text);
	}

	m_context.renderer->draw(m_texts);
}

void ControlsMenu::savePlayerConfig() const
{
	const auto& keyBindings = m_context.inputManager->getKeyBindings();
	const auto keyBindingsFileDoc = new rapidxml::xml_document();

	const auto keyBindingsNode = keyBindingsFileDoc->allocate_node(rapidxml::node_type::node_element, "KeyBindings");

	for (const auto& keyBinding : keyBindings)
	{
		const auto keyBindingNode = keyBindingsFileDoc->allocate_node(rapidxml::node_type::node_element, "KeyBinding");
		auto keyBindingNodeAttributeValue = keyBindingsFileDoc->allocate_string(keyBinding->getKeyBinding().c_str());
		keyBindingNode->append_attribute(keyBindingsFileDoc->allocate_attribute("key", keyBindingNodeAttributeValue));
		keyBindingNodeAttributeValue = keyBindingsFileDoc->allocate_string(
			std::to_string(keyBinding->getKeyBindingCharacter()).c_str());
		keyBindingNode->append_attribute(keyBindingsFileDoc->allocate_attribute("value", keyBindingNodeAttributeValue));
		keyBindingsNode->append_node(keyBindingNode);
	}

	keyBindingsFileDoc->append_node(keyBindingsNode);

	std::ofstream keyBindingsFileStream(FileConstants::KEYBINDINGS_SETTINGS_PATH);
	keyBindingsFileStream << *keyBindingsFileDoc;
	keyBindingsFileStream.close();
	keyBindingsFileDoc->clear();
	delete keyBindingsFileDoc;
}


std::string ControlsMenu::sVirtualKeyCodeToString(const int virtualKey)
{
	const LONG scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	TCHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
	case VK_RCONTROL:
	case VK_RMENU:
	case VK_LWIN:
	case VK_RWIN:
	case VK_APPS:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_INSERT:
	case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		KF_EXTENDED;
		break;
	default:
		result = GetKeyNameText(scanCode << 16, szName, 128);
		break;
	}
	if (result == 0)
	{
		const auto dwErrVal = static_cast<int>(GetLastError());
		throw std::system_error(std::error_code(dwErrVal, std::system_category()),
		                        "WinAPI Error occurred.");
	}
	return szName;
}
