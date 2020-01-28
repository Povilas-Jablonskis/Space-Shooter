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

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"

namespace Engine
{
	void MenuManager::savePlayerConfig(irrklang::ISoundEngine* soundEngine) const
	{
		auto doc = new rapidxml::xml_document<>();

		auto volume = doc->allocate_node(rapidxml::node_element, "Volume");
		const auto attribute_value = doc->allocate_string(std::to_string(soundEngine->getSoundVolume()).c_str());
		volume->append_attribute(doc->allocate_attribute("value", attribute_value));
		doc->append_node(volume);

		std::ofstream file_stored("Config/soundSettings.xml");
		file_stored << doc;
		file_stored.close();
		doc->clear();
		delete doc;
	}
	
	void MenuManager::loadPlayerModels(const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		m_playerModels.clear();

		auto doc = new rapidxml::xml_document<>();
		// Read the xml file into a vector
		std::ifstream theFile("Config/players.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(&buffer[0]);
		// Find our root node
		rapidxml::xml_node<>* root_node = doc->first_node("Players");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->next_sibling("Player"))
		{
			std::string spriteName = brewery_node->first_attribute("spriteName")->value();

			m_playerModels.push_back(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
		}

		theFile.close();
		doc->clear();
		delete doc;
	}

	void MenuManager::initGameMenus(irrklang::ISoundEngine* soundEngine, InputManager* inputManager, const std::shared_ptr<GameStateManager>& gameStateManager, const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		getMenus()->clear();

		auto mainMenu = std::make_shared<Menu>();
		auto playerModels = getPlayerModels();

		//Main Menu
		auto option = std::make_shared<Text>("Start Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
		option->onMouseReleaseFunc = [this, soundEngine, &gameStateManager, &spriteSheetManager, playerModels]()
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
					setCharacterSelectionIndex(static_cast<int>(playerModels.size()) - 1);
				}

				option2->applyAnimation(playerModels[getCharacterSelectionIndex()]);
			};
			characterSelection->addText(option);
			option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(49.0f, 60.0f));
			option->onMouseReleaseFunc = [this, soundEngine, option2, playerModels]()
			{
				soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

				setCharacterSelectionIndex(getCharacterSelectionIndex() + 1);

				if (getCharacterSelectionIndex() > static_cast<int>(playerModels.size()) - 1)
				{
					setCharacterSelectionIndex(static_cast<int>(playerModels.size()) - 1);
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
			option->onMouseReleaseFunc = [this, soundEngine, &gameStateManager, &spriteSheetManager]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				m_levelManager = std::make_shared<LevelManager>(spriteSheetManager, soundEngine, getCharacterSelectionIndex());
				getMenus()->clear();

				gameStateManager->setGameState(STARTED);
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
				auto i = 0.0f;
				for (const auto& keybinding : keybindings)
				{
					auto option = std::make_shared<Text>(keybinding.first + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f - 3.0f * i));
					option->disable();
					controls->addText(option);
					option = std::make_shared<Text>(InputManager::virtualKeyCodeToString(keybinding.second), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(29.0f, 60.0f - 3.0f * i), std::make_shared<KeyBindingInputComponent>(keybinding.first));
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
				option->onMouseReleaseFunc = [soundEngine, uniqueOption, this]()
				{
					soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

					const auto vol = soundEngine->getSoundVolume() - 0.01f;
					if (vol >= 0.0f)
					{
						soundEngine->setSoundVolume(vol);
						uniqueOption->setText(std::to_string(static_cast<int>(vol * 100.f)));
						savePlayerConfig(soundEngine);
					}
				};
				sounds->addText(option);
				option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(36.0f, 60.0f));
				option->onMouseReleaseFunc = [soundEngine, uniqueOption, this]()
				{
					soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);
					const auto vol = soundEngine->getSoundVolume() + 0.01f;
					if (vol <= 1.0f)
					{
						soundEngine->setSoundVolume(vol);
						uniqueOption->setText(std::to_string(static_cast<int>(vol * 100.f)));
						savePlayerConfig(soundEngine);
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

	void MenuManager::escapeAction(irrklang::ISoundEngine* soundEngine, InputManager* inputManager, const std::shared_ptr<GameStateManager>& gameStateManager, const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		if (gameStateManager->getGameState() == ENDED)
		{
			initGameMenus(soundEngine, inputManager, gameStateManager, spriteSheetManager);
			gameStateManager->setGameState(IN_MENU);
		}
		else if (gameStateManager->getGameState() == IN_PAUSED_MENU)
		{
			getMenus()->clear();
			gameStateManager->setGameState(STARTED);
		}
		else if (gameStateManager->getGameState() == STARTED)
		{
			getMenus()->clear();

			auto pauseMenu = std::make_shared<Menu>();

			//Pause Menu
			auto option = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f));
			option->onMouseReleaseFunc = [this, soundEngine, inputManager, &gameStateManager, &spriteSheetManager]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

				initGameMenus(soundEngine, inputManager, gameStateManager, spriteSheetManager);
				gameStateManager->setGameState(IN_MENU);
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
			gameStateManager->setGameState(IN_PAUSED_MENU);
		}
		else if (gameStateManager->getGameState() == IN_MENU)
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

	void MenuManager::renderCurrentMenu(const std::shared_ptr<Renderer>& renderer, const float dt, const std::shared_ptr<ConfigurationManager>& configurationManager, const std::shared_ptr<InputManager>& inputManager)
	{
		auto back = getMenus()->back();
		const auto uiElements = back->getElements();
		const auto texts = back->getTexts();

		for (auto& text : *texts)
		{
			text->update(configurationManager, inputManager);
		}

		for (auto& uiElement : *uiElements)
		{
			uiElement->update(dt, inputManager);
		}

		renderer->draw(*texts);

		for (auto& uiElement : *uiElements)
		{
			renderer->draw(uiElement);
		}
	}
}