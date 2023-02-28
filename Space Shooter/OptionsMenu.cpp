#include "OptionsMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

OptionsMenu::OptionsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void OptionsMenu::onCreate()
{
	//Options
	const auto controlsOption = std::make_shared<Text>("Controls", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                   glm::vec2(48.0f, 60.0f), *m_context.font);
	controlsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/2.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::CONTROLS);
	};
	m_texts.push_back(controlsOption);
	const auto soundsOption = std::make_shared<Text>("Sounds", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                 glm::vec2(48.0f, 55.0f), *m_context.font);
	soundsOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/2.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::SOUNDS);
	};
	m_texts.push_back(soundsOption);
	const auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(48.0f, 50.0f),
	                                               *m_context.font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
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
	if (m_context.inputManager->isKeyActive(27))
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	}
}

void OptionsMenu::draw(float)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.inputManager);
	}

	m_context.renderer->draw(m_texts);
}
