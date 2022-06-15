#include "Application.hpp"
#include "Renderer.hpp"
#include "SpriteSheetManager.hpp"
#include "InputManager.hpp"
#include "FileConstants.hpp"
#include "SceneStateMachine.hpp"
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

Application::Application()
{
	getRenderer()->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
	getRenderer()->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

	loadPlayerConfig();
	getSpritesheetManager()->loadSpriteSheetsFromConfig();

	m_sceneManager->add(ScenesEnum::MAIN, std::make_shared<MainMenu>(m_sceneManager, m_soundEngine, m_inputManager));

	auto pickYourCharacterMenu = std::make_shared<PickYourCharacterMenu>(m_sceneManager, m_spriteSheetManager, m_inputManager, m_soundEngine);
	pickYourCharacterMenu->loadPlayerModels(m_spriteSheetManager);

	m_sceneManager->add(ScenesEnum::PICK_YOUR_CHARACTER, pickYourCharacterMenu);

	m_sceneManager->add(ScenesEnum::PAUSED, std::make_shared<PauseMenu>(m_sceneManager, m_soundEngine, m_inputManager));
	m_sceneManager->add(ScenesEnum::GAME_OVER, std::make_shared<GameOver>(m_sceneManager, m_inputManager));
	m_sceneManager->add(ScenesEnum::GAME_WON, std::make_shared<GameWon>(m_sceneManager, m_inputManager));

	m_sceneManager->add(ScenesEnum::OPTIONS, std::make_shared<OptionsMenu>(m_sceneManager, m_soundEngine, m_inputManager));
	m_sceneManager->add(ScenesEnum::SOUNDS, std::make_shared<SoundsMenu>(m_sceneManager, m_soundEngine, m_inputManager));
	m_sceneManager->add(ScenesEnum::CONTROLS, std::make_shared<ControlsMenu>(m_sceneManager, m_soundEngine, m_inputManager));

	getSceneManager()->switchTo(ScenesEnum::MAIN);
}

void Application::loadPlayerConfig() const
{
	{
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
				getInputManager()->addKeyBinding(std::make_shared<KeyBinding>(beer_node->first_attribute("key")->value(), std::stoi(beer_node->first_attribute("value")->value())));
			}
		}

		theFile.close();
		doc->clear();
		delete doc;
	}

	{
		auto doc = new rapidxml::xml_document<>();
		// Read the xml file into a vector
		std::ifstream theFile(FileConstants::soundSettingsPath);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(&buffer[0]);

		for (auto brewery_node = doc->first_node("SoundSettings"); brewery_node; brewery_node = brewery_node->next_sibling("SoundSettings"))
		{
			for (auto beer_node = brewery_node->first_node("Volume"); beer_node; beer_node = beer_node->next_sibling("Volume"))
			{
				getSoundEngine()->setSoundVolume(std::stof(beer_node->first_attribute("value")->value()));
			}
		}

		theFile.close();
		doc->clear();
		delete doc;
	}
}

void Application::render() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	m_sceneManager->processInput();
	m_sceneManager->draw(m_renderer, m_dt);

	glutSwapBuffers();
}