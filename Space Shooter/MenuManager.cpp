#include "MenuManager.hpp"
#include "Menu.hpp"
#include "Renderer.hpp"
#include "ConfigurationManager.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "GameStateManager.hpp"
#include "SpriteSheetManager.hpp"
#include "SpriteSheet.hpp"
#include "KeyBindingInputComponent.hpp"
#include "Text.hpp"

namespace Engine
{
	void MenuManager::loadPlayerModels(const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		m_playerModels.clear();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/players.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Players");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->next_sibling("Player"))
		{
			std::string spriteName = brewery_node->first_attribute("spriteName")->value();

			m_playerModels.push_back(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
		}

		theFile.close();
		doc.clear();
	}

	void MenuManager::initGameMenus(irrklang::ISoundEngine* soundEngine, InputManager* inputManager, const std::unique_ptr<LevelManager>& levelManager, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		getMenus()->clear();

		auto mainMenu = std::make_shared<Menu>();
		auto playerModels = getPlayerModels();

		//Main Menu
		auto option = std::make_shared<Text>("Start Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
		option->onMouseReleaseFunc = [this, soundEngine, &gameStateManager, &levelManager, &spriteSheetManager, playerModels]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			auto characterSelection = std::make_shared<Menu>();

			//Character Selection
			auto option = std::make_shared<Text>("Pick your character :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f));
			option->disable();
			characterSelection->addText(option);
			auto option2 = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(43.0f, 60.0f));
			option2->applyAnimation(playerModels[getCharacterSelectionIndex()]);
			option2->setScale(0.5f);
			option = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(40.0f, 60.0f));
			option->onMouseReleaseFunc = [this, soundEngine, option2, playerModels]()
			{
				soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

				setCharacterSelectionIndex(getCharacterSelectionIndex() - 1);

				if (getCharacterSelectionIndex() < 0)
				{
					setCharacterSelectionIndex(playerModels.size() - 1);
				}

				option2->applyAnimation(playerModels[getCharacterSelectionIndex()]);
			};
			characterSelection->addText(option);
			option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(49.0f, 60.0f));
			option->onMouseReleaseFunc = [this, soundEngine, option2, playerModels]()
			{
				soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

				setCharacterSelectionIndex(getCharacterSelectionIndex() + 1);

				if (getCharacterSelectionIndex() > (playerModels.size() - 1))
				{
					setCharacterSelectionIndex(playerModels.size() - 1);
				}

				option2->applyAnimation(playerModels[getCharacterSelectionIndex()]);
			};
			characterSelection->addText(option);
			characterSelection->addUIElement(option2);
			option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 20.0f));
			option->onMouseReleaseFunc = [this, soundEngine]()
			{
				soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

				getMenus()->pop_back();
			};
			characterSelection->addText(option);
			option = std::make_shared<Text>("Start", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(25.0f, 20.0f));
			option->onMouseReleaseFunc = [this, soundEngine, &gameStateManager, &levelManager, &spriteSheetManager]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				levelManager->initGameLevels(spriteSheetManager, soundEngine, getCharacterSelectionIndex());
				getMenus()->clear();

				gameStateManager->setGameState(GameState::STARTED);
			};
			characterSelection->addText(option);

			addMenu(characterSelection);
		};
		mainMenu->addText(option);
		option = std::make_shared<Text>("Options", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f));
		option->onMouseReleaseFunc = [soundEngine, this, inputManager]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			auto options = std::make_shared<Menu>();

			//Options
			auto option = std::make_shared<Text>("Controls", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
			option->onMouseReleaseFunc = [soundEngine, this, inputManager]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				auto controls = std::make_shared<Menu>();

				//Controls
				const auto keybindings = *inputManager->getKeyBindings();
				float i = 0.0f;
				for (auto keybinding : keybindings)
				{
					auto option = std::make_shared<Text>(keybinding.first + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f - (3.0f * i)));
					option->disable();
					controls->addText(option);
					option = std::make_shared<Text>(InputManager::virtualKeyCodeToString(keybinding.second), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(29.0f, 60.0f - (3.0f * i)), std::make_shared<KeyBindingInputComponent>(keybinding.first));
					option->onMouseReleaseFunc = [soundEngine, inputManager, option, keybinding]()
					{
						if (inputManager->getCurrentlyEditedKeyBinding().empty())
						{
							soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

							inputManager->setCurrentlyEditedKeyBinding(keybinding.first);

							option->disable();
							option->onHoverEnterFuncDefaults();
						}
					};
					controls->addText(option);
					i += 1.0f;
				}
				auto option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f));
				option->onMouseReleaseFunc = [this, soundEngine]()
				{
					soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

					getMenus()->pop_back();
				};
				controls->addText(option);

				addMenu(controls);
			};
			options->addText(option);
			option = std::make_shared<Text>("Sounds", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f));
			option->onMouseReleaseFunc = [this, soundEngine]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				auto sounds = std::make_shared<Menu>();

				//Sounds
				auto option = std::make_shared<Text>("Volume :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f));
				option->disable();
				sounds->addText(option);
				auto uniqueOption = std::make_shared<Text>(std::to_string(static_cast<int>(soundEngine->getSoundVolume() * 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(32.0f, 60.0f));
				uniqueOption->disable();
				option = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(29.0f, 60.0f));
				option->onMouseReleaseFunc = [soundEngine, uniqueOption]()
				{
					soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

					auto vol = soundEngine->getSoundVolume() - 0.01f;
					if (vol >= 0.0f)
					{
						soundEngine->setSoundVolume(vol);
						uniqueOption->setText(std::to_string(static_cast<int>(vol * 100.f)));
					}
				};
				sounds->addText(option);
				option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(36.0f, 60.0f));
				option->onMouseReleaseFunc = [soundEngine, uniqueOption]()
				{
					soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);
					auto vol = soundEngine->getSoundVolume() + 0.01f;
					if (vol <= 1.0f)
					{
						soundEngine->setSoundVolume(vol);
						uniqueOption->setText(std::to_string(static_cast<int>(vol * 100.f)));
					}
				};
				sounds->addText(option);
				sounds->addText(uniqueOption);
				option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f));
				option->onMouseReleaseFunc = [this, soundEngine]()
				{
					soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

					getMenus()->pop_back();
				};
				sounds->addText(option);

				addMenu(sounds);
			};
			options->addText(option);
			option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 54.0f));
			option->onMouseReleaseFunc = [this, soundEngine]()
			{
				soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

				getMenus()->pop_back();
			};
			options->addText(option);

			addMenu(options);
		};
		mainMenu->addText(option);
		option = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 54.0f));
		option->onMouseReleaseFunc = []()
		{
			#if _DEBUG
				std::cout << "exiting\n";
				getchar();
			#endif
			exit(0);
		};
		mainMenu->addText(option);

		addMenu(mainMenu);
	}

	void MenuManager::escapeAction(irrklang::ISoundEngine* soundEngine, InputManager* inputManager, const std::unique_ptr<LevelManager>& levelManager, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		if (gameStateManager->getGameState() == GameState::ENDED)
		{
			initGameMenus(soundEngine, inputManager, levelManager, gameStateManager, spriteSheetManager);
			gameStateManager->setGameState(GameState::IN_MENU);
		}
		else if (gameStateManager->getGameState() == GameState::IN_PAUSED_MENU)
		{
			getMenus()->clear();
			gameStateManager->setGameState(GameState::STARTED);
		}
		else if (gameStateManager->getGameState() == GameState::STARTED)
		{
			getMenus()->clear();

			auto pauseMenu = std::make_shared<Menu>();

			//Pause Menu
			auto option = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
			option->onMouseReleaseFunc = [this, soundEngine, inputManager, &levelManager, &gameStateManager, &spriteSheetManager]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

				initGameMenus(soundEngine, inputManager, levelManager, gameStateManager, spriteSheetManager);
				gameStateManager->setGameState(GameState::IN_MENU);
			};
			pauseMenu->addText(option);

			option = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f));
			option->onMouseReleaseFunc = []()
			{
				#if _DEBUG
					std::cout << "exiting\n";
					getchar();
				#endif
				exit(0);
			};
			pauseMenu->addText(option);

			addMenu(pauseMenu);
			gameStateManager->setGameState(GameState::IN_PAUSED_MENU);
		}
		else if (gameStateManager->getGameState() == GameState::IN_MENU)
		{
			if (!inputManager->getCurrentlyEditedKeyBinding().empty())
			{
				return;
			}

			auto menus = getMenus();
			if (menus->size() > 1)
			{
				soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
				menus->pop_back();
			}
		}
	}

	void MenuManager::renderCurrentMenu(const std::unique_ptr<Renderer>& renderer, float dt, const std::unique_ptr<ConfigurationManager>& configurationManager, const std::unique_ptr<InputManager>& inputManager)
	{
		auto back = getMenus()->back();
		auto uiElements = back->getElements();
		auto texts = back->getTexts();

		for (auto it = texts->begin(); it != texts->end(); ++it)
		{
			(*it)->update(dt, configurationManager, inputManager);
		}

		for (auto it = uiElements->begin(); it != uiElements->end(); ++it)
		{
			(*it)->update(dt, inputManager);
		}

		for (auto it = texts->begin(); it != texts->end(); ++it)
		{
			renderer->draw(*it);
		}

		for (auto it = uiElements->begin(); it != uiElements->end(); ++it)
		{
			renderer->draw(*it);
		}
	}
}