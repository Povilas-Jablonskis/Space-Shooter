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
#include "Shader.hpp"
#include "KeyBinding.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"
#include <iostream>
#include <freeglut/freeglut.h>

Application::Application()
{
	m_context.m_inputManager = &m_inputManager;
	m_context.m_soundEngine = m_soundEngine;
	m_context.m_spriteSheet = &m_spriteSheet;
	m_context.m_renderer = &m_renderer;
	m_context.m_font = &m_font;

	m_renderer.addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
	m_renderer.addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

	loadPlayerConfig();

	m_sceneManager.add(MAIN, std::make_shared<MainMenu>(m_sceneManager, m_context));

	const auto pickYourCharacterMenu = std::make_shared<PickYourCharacterMenu>(m_sceneManager, m_context);
	pickYourCharacterMenu->loadPlayerModels();

	m_sceneManager.add(PICK_YOUR_CHARACTER, pickYourCharacterMenu);

	m_sceneManager.add(PAUSED, std::make_shared<PauseMenu>(m_sceneManager, m_context));
	m_sceneManager.add(GAME_OVER, std::make_shared<GameOver>(m_sceneManager, m_context));
	m_sceneManager.add(GAME_WON, std::make_shared<GameWon>(m_sceneManager, m_context));

	m_sceneManager.add(OPTIONS, std::make_shared<OptionsMenu>(m_sceneManager, m_context));
	m_sceneManager.add(SOUNDS, std::make_shared<SoundsMenu>(m_sceneManager, m_context));
	m_sceneManager.add(CONTROLS, std::make_shared<ControlsMenu>(m_sceneManager, m_context));

	m_sceneManager.switchTo(MAIN);
}

void Application::loadPlayerConfig() const
{
	const auto doc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::soundSettingsPath);
	std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(buffer.data());

	for (auto brewery_node = doc->first_node("SoundSettings"); brewery_node; brewery_node = brewery_node->next_sibling(
		     "SoundSettings"))
	{
		for (auto beer_node = brewery_node->first_node("Volume"); beer_node; beer_node = beer_node->
		     next_sibling("Volume"))
		{
			m_soundEngine->setSoundVolume(std::stof(beer_node->first_attribute("value")->value()));
		}
	}

	theFile.close();
	doc->clear();
	delete doc;
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
