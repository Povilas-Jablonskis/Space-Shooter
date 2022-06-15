#include "ControlsMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"
#include "KeyBinding.hpp"

ControlsMenu::ControlsMenu(std::shared_ptr<SceneStateMachine> sceneStateMachine, irrklang::ISoundEngine* soundEngine, std::shared_ptr<InputManager> inputManager)
    : m_sceneStateMachine(sceneStateMachine), m_soundEngine(soundEngine), m_inputManager(inputManager)
{

}

void ControlsMenu::onCreate()
{
	//Controls
	const auto& keybindings = *m_inputManager->getKeyBindings();
	auto i = 0.0f;
	for (const auto& keybinding : keybindings)
	{
		auto keybindingKey = std::make_shared<Text>(keybinding->getKeyBinding() + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f - 5.0f * i));
		keybindingKey->disable();
		m_texts.push_back(keybindingKey);
		auto keybindingValue = std::make_shared<Text>(InputManager::virtualKeyCodeToString(keybinding->getKeyBindingCharacter()), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(35.0f, 60.0f - 5.0f * i));
		keybindingValue->onMouseReleaseFunc = [=]()
		{
			if (m_inputManager->getCurrentlyEditedKeyBinding() == nullptr)
			{
				m_soundEngine->play2D("assets/Sounds/buttonselect/3.wav", GL_FALSE);

				m_inputManager->setCurrentlyEditedKeyBinding(keybinding);

				keybindingValue->disable();
				keybindingValue->onHoverEnterFuncDefaults();
			}
		};
		keybinding->setText(keybindingValue);
		m_keybindings.push_back(keybinding);
		i += 1.0f;
	}
	auto option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f));
	option->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::OPTIONS);
	};
	m_texts.push_back(option);
}

void ControlsMenu::onActivate()
{
	m_inputManager->clearEverything();
}

void ControlsMenu::onDestroy() { }

void ControlsMenu::processInput()
{
	if (m_inputManager->getCurrentlyEditedKeyBinding() == nullptr && m_inputManager->getKey(27))
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::OPTIONS);
	}
}

void ControlsMenu::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	for (auto& keybinding : m_keybindings)
	{
		keybinding->getText()->update(m_inputManager);
		keybinding->update(m_inputManager);
	}

    renderer->draw(m_texts);

    for (auto& keybinding : m_keybindings)
    {
        renderer->draw(keybinding->getText());
    }
}