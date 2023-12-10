#include "OptionsMenu.hpp"
#include "InputManager.hpp"

#include <Windows.h>

#include "Colors.hpp"
#include "Sounds.hpp"

OptionsMenu::OptionsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void OptionsMenu::onCreate()
{
	//Options
	const auto controlsOption = std::make_shared<Text>("Controls", Colors::DEFAULT_TEXT,
	                                                   glm::vec2(48.0f, 60.0f), *m_context.font);
	controlsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::CONTROLS);
	};
	m_texts.push_back(controlsOption);
	const auto soundsOption = std::make_shared<Text>("Sounds", Colors::DEFAULT_TEXT,
	                                                 glm::vec2(48.0f, 55.0f), *m_context.font);
	soundsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::SOUNDS);
	};
	m_texts.push_back(soundsOption);
	const auto backOption = std::make_shared<Text>("Back", Colors::DEFAULT_TEXT,
	                                               glm::vec2(48.0f, 50.0f),
	                                               *m_context.font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::MAIN);
	};
	m_texts.push_back(backOption);
}

void OptionsMenu::onDestroy()
{
}

void OptionsMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void OptionsMenu::processInput()
{
	if (m_context.inputManager->isKeyActive(VK_ESCAPE))
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::MAIN);
	}
}

void OptionsMenu::draw(float)
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
