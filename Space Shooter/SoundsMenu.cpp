#include "SoundsMenu.hpp"
#include "Configs.hpp"
#include "InputManager.hpp"

#include <Windows.h>
#include <fstream>

#include "Colors.hpp"
#include "rapidxml/rapidxml_ext.hpp"
#include "Sounds.hpp"

SoundsMenu::SoundsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void SoundsMenu::savePlayerConfig() const
{
	const auto soundFileDoc = new rapidxml::xml_document<>();

	const auto soundSettingsNode = soundFileDoc->allocate_node(rapidxml::node_type::node_element, "SoundSettings");

	const auto volumeNode = soundFileDoc->allocate_node(rapidxml::node_type::node_element, "Volume");
	const auto volumeNodeValue = soundFileDoc->
		allocate_string(std::to_string(m_context.soundEngine->getSoundVolume()).c_str());
	volumeNode->append_attribute(soundFileDoc->allocate_attribute("value", volumeNodeValue));
	soundSettingsNode->append_node(volumeNode);
	soundFileDoc->append_node(soundSettingsNode);

	std::ofstream soundFileStream(Configs::SOUND_SETTINGS_PATH);
	soundFileStream << *soundFileDoc;
	soundFileStream.close();
	soundFileDoc->clear();
	delete soundFileDoc;
}

void SoundsMenu::onCreate()
{
	//Sounds
	const auto volumeText = std::make_shared<Text>("Volume :", Colors::DEFAULT_TEXT,
	                                               glm::vec2(20.0f, 60.0f), *m_context.font);
	volumeText->disable();
	m_texts.push_back(volumeText);
	const auto volumeValueText = std::make_shared<Text>(
		std::to_string(static_cast<int>(m_context.soundEngine->getSoundVolume() * 100.0f)),
		Colors::DEFAULT_TEXT, glm::vec2(33.0f, 60.0f), *m_context.font);
	volumeValueText->disable();
	const auto arrowLeftText = std::make_shared<Text>("<", Colors::DEFAULT_TEXT,
	                                                  glm::vec2(30.0f, 60.0f),
	                                                  *m_context.font);
	arrowLeftText->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		const auto volume = m_context.soundEngine->getSoundVolume() - 0.01f;
		if (volume >= 0.0f)
		{
			m_context.soundEngine->setSoundVolume(volume);
			volumeValueText->setText(std::to_string(static_cast<int>(volume * 100.f)));
			savePlayerConfig();
		}
	};
	m_texts.push_back(arrowLeftText);
	const auto arrowRightText = std::make_shared<Text>(">", Colors::DEFAULT_TEXT,
	                                                   glm::vec2(37.0f, 60.0f),
	                                                   *m_context.font);
	arrowRightText->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_NEXT_OPTION, GL_FALSE);
		const auto volume = m_context.soundEngine->getSoundVolume() + 0.01f;
		if (volume <= 1.0f)
		{
			m_context.soundEngine->setSoundVolume(volume);
			volumeValueText->setText(std::to_string(static_cast<int>(volume * 100.f)));
			savePlayerConfig();
		}
	};
	m_texts.push_back(arrowRightText);
	m_texts.push_back(volumeValueText);
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

void SoundsMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void SoundsMenu::onDestroy()
{
}

void SoundsMenu::processInput()
{
	if (m_context.inputManager->isKeyActive(VK_ESCAPE))
	{
		m_context.soundEngine->play2D(Sounds::MENU_SELECT_GO_BACK_OPTION, GL_FALSE);

		m_sceneStateMachine.switchTo(SceneName::OPTIONS);
	}
}

void SoundsMenu::draw(float)
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
