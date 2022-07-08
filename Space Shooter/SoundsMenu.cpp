#include "SoundsMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "FileConstants.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"

#include <fstream>
#include "rapidxml/RapidXMLSTD.hpp"

SoundsMenu::SoundsMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
    : m_sceneStateMachine(sceneStateMachine), m_context(context)
{

}

void SoundsMenu::savePlayerConfig() const
{
	auto doc = new rapidxml::xml_document<>();

	auto SoundSettings = doc->allocate_node(rapidxml::node_type::node_element, "SoundSettings");

	auto volume = doc->allocate_node(rapidxml::node_type::node_element, "Volume");
	const auto attribute_value = doc->allocate_string(std::to_string(m_context.m_soundEngine->getSoundVolume()).c_str());
	volume->append_attribute(doc->allocate_attribute("value", attribute_value));
	SoundSettings->append_node(volume);
	doc->append_node(SoundSettings);

	std::ofstream file_stored(FileConstants::soundSettingsPath);
	file_stored << *doc;
	file_stored.close();
	doc->clear();
	delete doc;
}

void SoundsMenu::onCreate()
{
	//Sounds
	auto volumeText = std::make_shared<Text>("Volume :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f), *m_context.m_font);
	volumeText->disable();
	m_texts.push_back(volumeText);
	auto volumeValueText = std::make_shared<Text>(std::to_string(static_cast<int>(m_context.m_soundEngine->getSoundVolume() * 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(33.0f, 60.0f), *m_context.m_font);
	volumeValueText->disable();
	auto arrowLeftText = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(30.0f, 60.0f), *m_context.m_font);
	arrowLeftText->onMouseReleaseFunc = [=]()
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		const auto vol = m_context.m_soundEngine->getSoundVolume() - 0.01f;
		if (vol >= 0.0f)
		{
			m_context.m_soundEngine->setSoundVolume(vol);
			volumeValueText->setText(std::to_string(static_cast<int>(vol * 100.f)));
			savePlayerConfig();
		}
	};
	m_texts.push_back(arrowLeftText);
	auto arrowRightText = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(37.0f, 60.0f), *m_context.m_font);
	arrowRightText->onMouseReleaseFunc = [=]()
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);
		const auto vol = m_context.m_soundEngine->getSoundVolume() + 0.01f;
		if (vol <= 1.0f)
		{
			m_context.m_soundEngine->setSoundVolume(vol);
			volumeValueText->setText(std::to_string(static_cast<int>(vol * 100.f)));
			savePlayerConfig();
		}
	};
	m_texts.push_back(arrowRightText);
	m_texts.push_back(volumeValueText);
	auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f), *m_context.m_font);
	backOption->onMouseReleaseFunc = [=]()
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
	};
	m_texts.push_back(backOption);
}

void SoundsMenu::onActivate()
{
	m_context.m_inputManager->clearEverything();
}

void SoundsMenu::onDestroy() { }

void SoundsMenu::processInput()
{
	if (m_context.m_inputManager->getKey(27))
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::OPTIONS);
	}
}

void SoundsMenu::draw(float dt)
{
	for (auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);
}