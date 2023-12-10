#include "ControlsMenu.hpp"

#include "InputManager.hpp"

#include <freeglut/freeglut.h>
#include <magic_enum/magic_enum.hpp>

#include "Colors.hpp"
#include "Sounds.hpp"

ControlsMenu::ControlsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void ControlsMenu::onCreate()
{
	const auto bindableActionEntries = magic_enum::enum_entries<BindableAction>();

	//Controls
	auto index = 0.0f;
	for (const auto& [value, key] : bindableActionEntries)
	{
		const auto keybind = m_context.inputManager->getKeybind(value);

		const auto positionY = 60.0f - 5.0f * static_cast<float>(index);

		auto keybindKey = std::make_shared<Text>(std::string(key) + " :",
			Colors::DEFAULT_TEXT,
			glm::vec2(20.0f, positionY),
			*m_context.font);
		keybindKey->disable();
		m_texts.push_back(keybindKey);

		auto keybindText = keybind ? m_context.inputManager->virtualKeyCodeToString(keybind) : "Press any key";

		auto keybindValue = std::make_shared<Text>(keybindText,
			Colors::DEFAULT_TEXT,
			glm::vec2(35.0f, positionY),
			*m_context.font);
		keybindValue->onMouseReleaseFunc = [=, this]
		{
			if (m_currentlyEditedKeybind == nullptr)
			{
				m_currentlyEditedKeybind = std::make_unique<BindableAction>(value);

				keybindValue->disable();
				keybindValue->onHoverEnterFuncDefaults();
			}
		};
		m_texts.push_back(keybindValue);
		m_keybindsTexts.insert_or_assign(value, keybindValue);
		index += 1.0f;
	}

	const auto backOption = std::make_shared<Text>("Back", Colors::DEFAULT_TEXT,
	                                               glm::vec2(48.0f, 20.0f),
	                                               *m_context.font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::OPTIONS);
	};
	m_texts.push_back(backOption);
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
	if (m_currentlyEditedKeybind == nullptr)
	{
		if (m_context.inputManager->isKeyActive(VK_ESCAPE))
		{
			m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

			m_sceneStateMachine.switchTo(SceneName::OPTIONS);
		}
	}
	else
	{
		if (m_context.inputManager->isKeyActive(VK_ESCAPE))
		{
			const auto& currentlyEditedKeyBindingText = m_keybindsTexts.at(*m_currentlyEditedKeybind);

			currentlyEditedKeyBindingText->enable();
			currentlyEditedKeyBindingText->onHoverExitFuncDefaults();

			m_currentlyEditedKeybind = nullptr;
			m_context.inputManager->clearEverything();
			return;
		}

		const auto& keys = m_context.inputManager->getKeysStates();

		for (const auto& [key, isActive] : keys)
		{
			if (isActive)
			{
				if (key >= 32 && key < 127 && !m_context.inputManager->keybindingsContainKey(key))
				{
					const auto& currentlyEditedKeyBindingText = m_keybindsTexts.at(*m_currentlyEditedKeybind);

					currentlyEditedKeyBindingText->enable();
					currentlyEditedKeyBindingText->onHoverExitFuncDefaults();
					currentlyEditedKeyBindingText->setText(m_context.inputManager->virtualKeyCodeToString(key));

					m_context.inputManager->setKeybindCharacter(*m_currentlyEditedKeybind, key);
					m_currentlyEditedKeybind = nullptr;

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
