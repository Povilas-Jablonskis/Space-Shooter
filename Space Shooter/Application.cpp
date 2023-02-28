#include "Application.hpp"
#include "MainMenu.hpp"
#include "PickYourCharacterMenu.hpp"
#include "OptionsMenu.hpp"
#include "SoundsMenu.hpp"
#include "ControlsMenu.hpp"
#include "GameScene.hpp"
#include "PauseMenu.hpp"
#include "GameOver.hpp"
#include "GameWon.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"
#include <iostream>
#include <freeglut/freeglut.h>

Application::Application()
{
	m_context.inputManager = &m_inputManager;
	m_context.soundEngine = m_soundEngine;
	m_context.spriteSheet = &m_spriteSheet;
	m_context.renderer = &m_renderer;
	m_context.font = &m_font;

	m_renderer.addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
	m_renderer.addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

	loadPlayerConfig();

	m_sceneManager.add(ScenesEnum::MAIN, std::make_shared<MainMenu>(m_sceneManager, m_context));

	const auto pickYourCharacterMenu = std::make_shared<PickYourCharacterMenu>(m_sceneManager, m_context);
	pickYourCharacterMenu->loadPlayerModels();

	m_sceneManager.add(ScenesEnum::PICK_YOUR_CHARACTER, pickYourCharacterMenu);

	m_sceneManager.add(ScenesEnum::PAUSED, std::make_shared<PauseMenu>(m_sceneManager, m_context));
	m_sceneManager.add(ScenesEnum::GAME_OVER, std::make_shared<GameOver>(m_sceneManager, m_context));
	m_sceneManager.add(ScenesEnum::GAME_WON, std::make_shared<GameWon>(m_sceneManager, m_context));

	m_sceneManager.add(ScenesEnum::OPTIONS, std::make_shared<OptionsMenu>(m_sceneManager, m_context));
	m_sceneManager.add(ScenesEnum::SOUNDS, std::make_shared<SoundsMenu>(m_sceneManager, m_context));
	m_sceneManager.add(ScenesEnum::CONTROLS, std::make_shared<ControlsMenu>(m_sceneManager, m_context));

	m_sceneManager.switchTo(ScenesEnum::MAIN);
}

void Application::loadPlayerConfig() const
{
	const auto soundsFileDoc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream soundsFile(FileConstants::SOUND_SETTINGS_PATH);
	std::vector soundsFileBuffer((std::istreambuf_iterator(soundsFile)), std::istreambuf_iterator<char>());
	soundsFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	soundsFileDoc->parse<0>(soundsFileBuffer.data());

	for (auto soundSettingsNode = soundsFileDoc->first_node("SoundSettings"); soundSettingsNode; soundSettingsNode =
	     soundSettingsNode->next_sibling(
		     "SoundSettings"))
	{
		for (auto volumeNode = soundSettingsNode->first_node("Volume"); volumeNode; volumeNode = volumeNode->
		     next_sibling("Volume"))
		{
			m_soundEngine->setSoundVolume(std::stof(volumeNode->first_attribute("value")->value()));
		}
	}

	soundsFile.close();
	soundsFileDoc->clear();
	delete soundsFileDoc;
}

void Application::render() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_sceneManager.processInput();
	m_sceneManager.draw(m_dt);

	glutSwapBuffers();
}

InputManager& Application::getInputManager()
{
	return m_inputManager;
}
