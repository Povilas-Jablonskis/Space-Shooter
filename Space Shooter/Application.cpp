#include "Application.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include "rapidxml_print.hpp"
#include "Timer.h"

namespace Engine
{
	Application::Application() 
		: effectManager(std::make_shared<EffectManager>()), enemyManager(std::make_shared<EnemyManager>()), pickupManager(std::make_shared<PickupManager>()), spriteSheetManager(std::make_shared<SpriteSheetManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), inputManager(std::make_shared<InputManager>()), gameState(GameState::NOTSTARTEDYET), characterSelectionIndex(0)
	{
		soundEngine = irrklang::createIrrKlangDevice();

		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;
		t = 0.0f;

		srand((int)time(NULL));

		onNotify = [this](ObserverEvent _event, std::map<std::string, BaseGameObject*> params)
		{
			switch (_event)
			{
				case SCORECHANGED:
				{
					updatePlayerScore();
					break;
				}
				case HEALTHCHANGED:
				{
					updatePlayerHealth();
					break;
				}
				case BULLETDESTROYED:
				{
					auto bullet = params["bullet"];

					auto explosion = std::make_shared<Explosion>(32.0f, 32.0f, bullet->getPosition());
					explosion->applyAnimation(bullet->getAnimationByIndex("explosion"));
					addExplosionToList(std::move(explosion));
					break;
				}
				case BULLETSHOT:
				{
					auto subject = dynamic_cast<Entity*>(params["collider"]);
					soundEngine->play2D(subject->getShootingSound().c_str(), GL_FALSE);
					break;
				}
				case PLAYERDIED:
				{
					soundEngine->play2D("Sounds/sfx_lose.ogg", GL_FALSE);
					break;
				}
			}
		};

		loadPlayerModels();
		loadConfig();
		spriteSheetManager->loadSpriteSheetsFromConfig();
		effectManager->loadEffectsFromConfig(spriteSheetManager, soundEngine);
		pickupManager->loadPickupsFromConfig(spriteSheetManager, effectManager, soundEngine);
		enemyManager->loadEnemiesFromConfig(spriteSheetManager, effectManager);
		initGameUI();

		background = std::make_shared<UIElementBase>((float)glutGet(GLUT_INIT_WINDOW_WIDTH), (float)glutGet(GLUT_INIT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
		background->applyAnimation(spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));
	}

	Application::~Application()
	{
		enemiesColumns.clear();
		explosions.clear();
		notifications.clear();
		scoreBoard.clear();
		playerHealth.clear();
		pickups.clear();
		meteors.clear();

		saveConfig();
	}

	void Application::updatePlayerHealth()
	{
		playerHealth.clear();

		auto option = std::make_shared<UIElementBase>(33.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), glm::vec2(6.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(player->getHealthIcon()));
		playerHealth.push_back(std::move(option));

		option = std::make_shared<UIElementBase>(17.0f, 17.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), glm::vec2(12.0f, 92.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png"));
		playerHealth.push_back(std::move(option));

		option = std::make_shared<UIElementBase>(17.0f, 17.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), glm::vec2(16.0f, 92.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getHealth()) + ".png"));
		playerHealth.push_back(std::move(option));
	}

	void Application::updatePlayerScore()
	{
		scoreBoard.clear();

		auto str = std::to_string(player->getScore());

		for (size_t i = 0; i < str.size(); i++)
		{
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(17.0f, 17.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), glm::vec2(70.0f + (4 * (i + 1)), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			scoreBoard.push_back(std::move(option));
		}
	}

	void Application::resetScene()
	{
		explosions.clear();
		enemiesColumns.clear();
		pickups.clear();
		meteors.clear();
		notifications.clear();

		player = std::make_shared<Player>(32.0f, 32.0f, glm::vec2((float)glutGet(GLUT_WINDOW_WIDTH) / 2.0f, 0.0f), glm::vec2(80.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		effectManager->getEffect("defaultShooting")(player);
		player->onDeath = [this]()
		{
			auto option = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
			option->setIsStatic(true);
			notifications.push_back(std::move(uiPlayerElement("Game Over", option)));

			player->clearBullets();
			inputManager->resetInput();
			setState(GameState::ENDED);
		};
		playerModels[characterSelectionIndex].second();
		player->addObserver(this);

		currentLevel = 0;

		initScene();
		updatePlayerHealth();
		updatePlayerScore();

		accumulator = 0.0f;
		t = 0.0f;
	}

	void Application::initGameUI()
	{
		auto options = std::make_shared<menu>("Options", std::vector<std::shared_ptr<UIElementBase>>());
		auto mainMenu = std::make_shared<menu>("Main Menu", std::vector<std::shared_ptr<UIElementBase>>());
		auto pauseMenu = std::make_shared<menu>("Pause Menu", std::vector<std::shared_ptr<UIElementBase>>());
		auto controls = std::make_shared<menu>("Controls", std::vector<std::shared_ptr<UIElementBase>>());
		auto sounds = std::make_shared<menu>("Sounds", std::vector<std::shared_ptr<UIElementBase>>());
		auto characterSelection = std::make_shared<menu>("Character Selection", std::vector<std::shared_ptr<UIElementBase>>());

		//Main Menu
		auto option = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = getMenu("Character Selection");
		};
		mainMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = getMenu("Options");
		};
		mainMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
		option->onMouseReleaseFunc = [this]()
		{
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		mainMenu->second.push_back(std::move(option));

		//Pause Menu
		option = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

			currentMenu = getMenu("Main Menu");
			setState(GameState::NOTSTARTEDYET);
		};
		pauseMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		pauseMenu->second.push_back(std::move(option));

		//Controls
		auto keybindings = inputManager->getKeyBindings();
		size_t i = 0;
		for (auto it = keybindings->begin(); it != keybindings->end(); it++)
		{
			option = std::make_shared<Text>(it->first + " :", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
			option->setIsStatic(true);
			controls->second.push_back(std::move(option));
			option = std::make_shared<Text>(virtualKeyCodeToString(it->second), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
			option->onMouseReleaseFunc = [this, option, it]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

				inputManager->resetCurrentEditedKeyBinding();
				inputManager->setCurrentEditedKeyBinding(currentEditedKeyBinding(it, option));
				option->setIsStatic(true);
				option->changeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			};
			controls->second.push_back(std::move(option));
			i++;
		}
		option = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

			currentMenu = getMenu("Options");
		};
		controls->second.push_back(std::move(option));

		//Sounds
		option = std::make_shared<Text>("Volume :", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->setIsStatic(true);
		sounds->second.push_back(std::move(option));
		option = std::make_shared<Text>("<", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(70.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			float vol = soundEngine->getSoundVolume() - 0.01f;
			if (vol >= 0.0f)
			{
				soundEngine->setSoundVolume(vol);
				dynamic_cast<Text*>(getMenu("Sounds")->second[2].get())->setText(std::to_string((int)(vol * 100.f)));
			}
		};
		sounds->second.push_back(std::move(option));
		option = std::make_shared<Text>(std::to_string((int)(soundEngine->getSoundVolume() * 100.0f)), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(75.0f, 60.0f));
		option->setIsStatic(true);
		sounds->second.push_back(std::move(option));
		option = std::make_shared<Text>(">", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(85.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);
			float vol = soundEngine->getSoundVolume() + 0.01f;
			if (vol <= 1.0f)
			{
				soundEngine->setSoundVolume(vol);
				dynamic_cast<Text*>(getMenu("Sounds")->second[2].get())->setText(std::to_string((int)(vol * 100.f)));
			}
		};
		sounds->second.push_back(std::move(option));
		option = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			
			currentMenu = getMenu("Options");
		};
		sounds->second.push_back(std::move(option));

		//Options
		option = std::make_shared<Text>("Controls", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			
			currentMenu = getMenu("Controls");
		};
		options->second.push_back(std::move(option));
		option = std::make_shared<Text>("Sounds", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			
			currentMenu = getMenu("Sounds");
		};
		options->second.push_back(std::move(option));
		option = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			
			currentMenu = getMenu("Main Menu");
		};
		options->second.push_back(std::move(option));

		//Character Selection
		option = std::make_shared<Text>("Pick your character :", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f));
		option->setIsStatic(true);
		characterSelection->second.push_back(std::move(option));
		option = std::make_shared<Text>("<", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			characterSelectionIndex--;

			if (characterSelectionIndex < 0) characterSelectionIndex = (playerModels.size() - 1);

			getMenu("Character Selection")->second[2].get()->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(playerModels[characterSelectionIndex].first));
		};
		characterSelection->second.push_back(std::move(option));
		auto option2 = std::make_shared<UIElementBase>(32.0f, 32.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), glm::vec2(60.0f, 58.0f));
		option2->setIsStatic(true);
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(playerModels[characterSelectionIndex].first));
		characterSelection->second.push_back(std::move(option2));
		option = std::make_shared<Text>(">", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(73.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			characterSelectionIndex++;

			if (characterSelectionIndex > (playerModels.size() - 1)) characterSelectionIndex = 0;

			getMenu("Character Selection")->second[2].get()->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(playerModels[characterSelectionIndex].first));
		};
		characterSelection->second.push_back(std::move(option));
		option = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 20.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

			currentMenu = getMenu("Options");
		};
		characterSelection->second.push_back(std::move(option));
		option = std::make_shared<Text>("Start", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(29.0f, 20.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			resetScene();
			setState(GameState::STARTED);
		};
		characterSelection->second.push_back(std::move(option));

		gameMenu.push_back(std::move(mainMenu));
		gameMenu.push_back(std::move(pauseMenu));
		gameMenu.push_back(std::move(controls));
		gameMenu.push_back(std::move(sounds));
		gameMenu.push_back(std::move(options));
		gameMenu.push_back(std::move(characterSelection));

		currentMenu = getMenu("Main Menu");
	}

	bool Application::initScene()
	{
		size_t i = 0;
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/levels.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Levels");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->next_sibling("Level"))
		{
			if (i == currentLevel)
			{
				std::string::size_type sz;
				for (auto beer_node = brewery_node->first_node("Column"); beer_node; beer_node = beer_node->next_sibling("Column"))
				{
					auto enemyColumn = std::make_shared<EnemyColumn>(glm::vec2(std::stof(beer_node->first_attribute("maxPositionX")->value(), &sz), std::stof(beer_node->first_attribute("maxPositionY")->value(), &sz)));
					for (auto beer_node2 = beer_node->first_node("Enemy"); beer_node2; beer_node2 = beer_node2->next_sibling("Enemy"))
					{
						float positionX = std::stof(beer_node2->first_attribute("positionX")->value(), &sz);
						float positionY = std::stof(beer_node2->first_attribute("positionY")->value(), &sz);

						auto enemy = enemyManager->getEnemy(beer_node2->first_attribute("name")->value());
						enemy->setPosition(glm::vec2(positionX, positionY));
						enemy->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value(), &sz), std::stof(beer_node->first_attribute("velocityY")->value(), &sz)));
						enemy->addObserver(this);
						enemyColumn->addEnemy(std::move(enemy));
					}
					enemiesColumns.push_back(std::move(enemyColumn));
				}
				for (auto beer_node = brewery_node->first_node("Meteor"); beer_node; beer_node = beer_node->next_sibling("Meteor"))
				{
					float positionX = std::stof(beer_node->first_attribute("positionX")->value(), &sz);
					float positionY = std::stof(beer_node->first_attribute("positionY")->value(), &sz);

					auto meteor = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(positionX, positionY), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
					meteor->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node->first_attribute("name")->value()));
					meteor->onDeath = [this, meteor]()
					{
						if ((rand() % 2) == 0) return;

						auto pickup = pickupManager->getRandomPickup();
						pickup->setPosition(meteor->getPosition());
						pickups.push_back(std::move(pickup));
					};
					meteor->onCollision = [this, meteor](std::shared_ptr<BaseGameObject> collider)
					{
						meteor->setNeedsToBeRemoved(true);

						auto entity = dynamic_cast<Entity*>(collider.get());

						if (entity != nullptr && !entity->getNeedsToBeRemoved())
						{
							if (entity->getAddon("shield") != nullptr)
								entity->getAddon("shield")->setNeedsToBeRemoved(true);
							else
								entity->setNeedsToBeRemoved(true);
						}
					};
					meteor->setValue(100);
					meteors.push_back(std::move(meteor));
				}
				return true;
			}
			i++;
		}
		return false;
	}

	void Application::loadPlayerModels()
	{
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
			std::string healthIcon = std::string(brewery_node->first_attribute("healthIcon")->value());
			std::string spriteName = std::string(brewery_node->first_attribute("spriteName")->value());
			std::map<std::string, std::string> sprites;
			std::map<std::string, std::string> animations;

			for (auto beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
			{
				for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
				{
					sprites.insert(std::pair<std::string, std::string>(beer_node2->first_attribute("name")->value(), beer_node2->first_attribute("spriteName")->value()));
				}

				for (auto beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
				{
					animations.insert(std::pair<std::string, std::string>(beer_node2->first_attribute("name")->value(), beer_node2->first_attribute("animationName")->value()));
				}
			}

			auto _effect = [this, sprites, spriteName, animations, healthIcon]()
			{
				for (auto sprite : sprites)
				{
					player->addAnimation(sprite.first, std::move(spriteSheetManager->getSpriteSheet("main")->getSprite(sprite.second)));
				}

				for (auto animation : animations)
				{
					player->addAnimation(animation.first, std::move(spriteSheetManager->getSpriteSheet("main")->getAnimation(animation.second)));
				}

				player->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
				player->setHealthIcon(healthIcon);
			};
			playerModels.push_back(std::move(playerModel(std::string(brewery_node->first_attribute("name")->value()), std::move(_effect))));
		}
	}

	void Application::saveConfig()
	{
		typedef std::pair<std::string, std::string> keyBinding;

		auto keyBindings = inputManager->getKeyBindings();
		auto keyBindings2 = std::vector<keyBinding>();
		auto shaders = renderer->getShaders();
		auto fonts = fontManager->getFonts();
		rapidxml::xml_document<> doc;

		rapidxml::xml_node<>* Config = doc.allocate_node(rapidxml::node_element, "Config");
		doc.append_node(Config);

		rapidxml::xml_node<>* KeyBindings = doc.allocate_node(rapidxml::node_element, "KeyBindings");

		for (auto it = keyBindings->begin(); it != keyBindings->end(); it++)
		{
			keyBindings2.push_back(std::move(keyBinding(it->first, std::to_string(it->second))));
		}

		for (auto it = keyBindings2.begin(); it != keyBindings2.end(); it++)
		{
			rapidxml::xml_node<>* KeyBinding = doc.allocate_node(rapidxml::node_element, "KeyBinding");
			KeyBinding->append_attribute(doc.allocate_attribute("key", it->first.c_str()));
			KeyBinding->append_attribute(doc.allocate_attribute("value", it->second.c_str()));
			KeyBindings->append_node(KeyBinding);
		}

		Config->append_node(KeyBindings);
		rapidxml::xml_node<>* Shaders = doc.allocate_node(rapidxml::node_element, "Shaders");

		for (auto it = shaders->begin(); it != shaders->end(); it++)
		{
			rapidxml::xml_node<>* Shader = doc.allocate_node(rapidxml::node_element, "Shader");
			Shader->append_attribute(doc.allocate_attribute("key", it->first.c_str()));
			Shaders->append_node(Shader);
		}

		Config->append_node(Shaders);
		rapidxml::xml_node<>* Fonts = doc.allocate_node(rapidxml::node_element, "Fonts");

		for (auto it = fonts->begin(); it != fonts->end(); it++)
		{
			rapidxml::xml_node<>* Font = doc.allocate_node(rapidxml::node_element, "Font");
			Font->append_attribute(doc.allocate_attribute("key", it->first.c_str()));
			Fonts->append_node(Font);
		}

		Config->append_node(Fonts);

		rapidxml::xml_node<>* Volume = doc.allocate_node(rapidxml::node_element, "Volume");
		std::string vol = std::to_string(soundEngine->getSoundVolume());
		Volume->append_attribute(doc.allocate_attribute("value", vol.c_str()));
		Config->append_node(Volume);

		std::ofstream file_stored("Config/config.xml");
		file_stored << doc;
		file_stored.close();
		doc.clear();
	}

	void Application::loadConfig()
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/config.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Config");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("KeyBindings"); brewery_node; brewery_node = brewery_node->next_sibling("KeyBindings"))
		{
			for (auto beer_node = brewery_node->first_node("KeyBinding"); beer_node; beer_node = beer_node->next_sibling("KeyBinding"))
			{
				inputManager->setKeyBinding(beer_node->first_attribute("key")->value(), std::stoi(beer_node->first_attribute("value")->value()));
			}
		}

		for (auto brewery_node = root_node->first_node("Fonts"); brewery_node; brewery_node = brewery_node->next_sibling("Fonts"))
		{
			for (auto beer_node = brewery_node->first_node("Font"); beer_node; beer_node = beer_node->next_sibling("Font"))
			{
				std::string key = beer_node->first_attribute("key")->value();
				fontManager->loadFont(key + ".ttf", key);
			}
		}

		for (auto brewery_node = root_node->first_node("Shaders"); brewery_node; brewery_node = brewery_node->next_sibling("Shaders"))
		{
			for (auto beer_node = brewery_node->first_node("Shader"); beer_node; beer_node = beer_node->next_sibling("Shader"))
			{
				std::string key = beer_node->first_attribute("key")->value();
				renderer->addShader(key, std::move(std::make_shared<Shader>(key + ".vert", key + ".frag")));
			}
		}

		for (auto brewery_node = root_node->first_node("Volume"); brewery_node; brewery_node = brewery_node->next_sibling("Volume"))
		{
			soundEngine->setSoundVolume(std::stof(brewery_node->first_attribute("value")->value()));
		}
	}

	void Application::startNewLevel()
	{
		notifications.clear();
		currentLevel++;
		if (!initScene()) player->onDeath();
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
		float frameTime = (newTime - currentTime) / 1000.0f;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			inputManager->fixInput();
			if (getState() == GameState::STARTED)
			{
				inputManager->updatePlayerInput(player.get(), dt);
				player->update(dt);
			}

			if (getState() != GameState::NOTSTARTEDYET && getState() != GameState::PAUSED)
			{
				for (auto it = enemiesColumns.begin(); it != enemiesColumns.end();)
				{
					if ((*it)->update(dt))
					{
						it = enemiesColumns.erase(it);
						if (enemiesColumns.size() == 0)
						{
							Timer::windowsTimer([this] { startNewLevel(); }, 2000);

							auto option = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
							option->setIsStatic(true);
							notifications.push_back(std::move(uiPlayerElement("Level completed", option)));
						}
					}
					else
						++it;
				}

				for (auto it = explosions.begin(); it != explosions.end();)
				{
					if ((*it)->update(dt))
						it = explosions.erase(it);
					else
						++it;
				}

				for (auto it = pickups.begin(); it != pickups.end();)
				{
					if ((*it)->update(dt))
						it = pickups.erase(it);
					else
						++it;
				}

				for (auto it = meteors.begin(); it != meteors.end();)
				{
					if ((*it)->update(dt))
						it = meteors.erase(it);
					else
						++it;
				}
			}

			if (getState() == GameState::STARTED)
			{
				//Collision detection
				if (player->getPosition(0) + player->getSize(0) >= windowWidth)
					player->setPosition(0, windowWidth - player->getSize(0));
				else if (player->getPosition(0) <= 0.0f)
					player->setPosition(0, 0.0f);

				if (player->getPosition(1) + player->getSize(1) >= windowHeigth)
					player->setPosition(1, windowHeigth - player->getSize(1));
				else if (player->getPosition(1) <= 0.0f)
					player->setPosition(1, 0.0f);

				for (size_t i = 0; i < enemiesColumns.size(); i++)
				{
					auto enemies = enemiesColumns[i]->getEnemies();
					for (auto it = enemies->begin(); it != enemies->end(); it++)
					{
						auto playerBulletList = player->getBulletsList();
						auto enemyBulletList = (*it)->getBulletsList();
						for (auto it2 = playerBulletList->begin(); it2 != playerBulletList->end(); it2++)
						{
							collisionManager->checkCollision(*it2, enemyBulletList, *it);
						}
						collisionManager->checkCollision(player, (*it)->getBulletsList(), *it);
						collisionManager->checkCollision(*it, playerBulletList, player);
					}
				}

				for (auto it = meteors.begin(); it != meteors.end(); it++)
				{
					auto playerBulletList = player->getBulletsList();
					collisionManager->checkCollision(*it, playerBulletList, player);
				}

				collisionManager->checkCollision(player, &meteors);
				for (size_t i = 0; i < enemiesColumns.size(); i++)
				{
					auto enemies = enemiesColumns[i]->getEnemies();
					collisionManager->checkCollision(player, enemies);
				}
				collisionManager->checkCollision(player, &pickups);
			}

			if (getState() == GameState::STARTED)
				t += dt;

			accumulator -= dt;
		}

		if (getState() != GameState::NOTSTARTEDYET && getState() != GameState::PAUSED)
		{
			//Render background
			background->draw(renderer);
			//Render meteors
			for (auto meteor : meteors)
			{
				meteor->draw(renderer);
			}
			//Render player
			player->draw(renderer);
			auto addons = player->getAddons();
			for (auto it = addons->begin(); it != addons->end(); it++)
			{
				it->second->draw(renderer);
			}
			//Render enemies
			for (size_t i = 0; i < enemiesColumns.size(); i++)
			{
				auto enemies = enemiesColumns[i]->getEnemies();
				for (auto it = enemies->begin(); it != enemies->end(); it++)
				{
					auto addons = (*it)->getAddons();
					(*it)->draw(renderer);
					for (auto it2 = addons->begin(); it2 != addons->end(); it2++)
					{
						it2->second->draw(renderer);
					}
				}
			}
			//Render pickups
			for (auto pickup : pickups)
			{
				pickup->draw(renderer);
			}
			//Render explosions
			for (auto explosion : explosions)
			{
				explosion->draw(renderer);
			}
			//Render bullets
			for (size_t i = 0; i < enemiesColumns.size(); i++)
			{
				auto enemies = enemiesColumns[i]->getEnemies();
				for (auto it = enemies->begin(); it != enemies->end(); it++)
				{
					auto enemyBulletList = (*it)->getBulletsList();
					for (auto it2 = enemyBulletList->begin(); it2 != enemyBulletList->end(); it2++)
					{
						(*it2)->draw(renderer);
					}
				}
			}

			auto playerBulletList = player->getBulletsList();
			for (auto it = playerBulletList->begin(); it != playerBulletList->end(); it++)
			{
				(*it)->draw(renderer);
			}
			//Render & Update player UI
			for (auto uiElement : playerHealth)
			{
				uiElement->update(dt);
				uiElement->draw(renderer);
			}

			for (auto uiElement : scoreBoard)
			{
				uiElement->update(dt);
				uiElement->draw(renderer);
			}

			for (auto uiElement : notifications)
			{
				uiElement.second->update(dt);
				uiElement.second->draw(renderer);
			}
		}
		else if (getState() == GameState::NOTSTARTEDYET || getState() == GameState::PAUSED)
		{
			for (auto uiElement : currentMenu->second)
			{
				uiElement->update(dt);
				uiElement->draw(renderer);
			}
		}

		glutSwapBuffers();
	}

	void Application::resize(int width, int height)
	{
		//const float ar = (float) width / (float) height;

		for (auto menu : gameMenu)
		{
			for (auto uiElement : menu->second)
			{
				uiElement->fixPosition();
			}
		}

		for (auto uiElement : playerHealth)
			uiElement->fixPosition();

		for (auto uiElement : scoreBoard)
			uiElement->fixPosition();

		for (auto uiElement : notifications)
			uiElement.second->fixPosition();

		background->fixPosition();

		#if _DEBUG
			glutReshapeWindow(width, height);
			glViewport(0, 0, width, height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
			return;
		#endif

		width = glutGet(GLUT_INIT_WINDOW_WIDTH);
		height = glutGet(GLUT_INIT_WINDOW_HEIGHT);

		glutReshapeWindow(width, height);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	}

	void Application::keyboardInputUp(unsigned char c, int x, int y)
	{
		auto key = VkKeyScan(c);
		if (inputManager->getKey(key))
			inputManager->setKey(key, false);
	}

	void Application::keyboardInput(unsigned char c, int x, int y)
	{
		auto key = VkKeyScan(c);
		if (!inputManager->getKey(key))
		{
			switch (c)
			{
				case 27:
				{
					if (getState() == GameState::ENDED)
					{
						currentMenu = getMenu("Main Menu");
						setState(GameState::NOTSTARTEDYET);
					}
					else if (getState() == GameState::PAUSED)
					{
						setState(GameState::STARTED);
					}
					else if (getState() == GameState::STARTED)
					{
						currentMenu = getMenu("Pause Menu");
						setState(GameState::PAUSED);
					}
					else if (getState() == GameState::NOTSTARTEDYET)
					{
						if (!inputManager->resetCurrentEditedKeyBinding())
						{
							currentMenu->second.back()->onMouseReleaseFunc();
						}
					}
					break;
				}
			}

			auto keyBindings = inputManager->getKeyBindings();
			auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
			if (c >= 32 && c < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [key](keybinding element){return element.second == key; }) && currentKeyBinding->second != nullptr)
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
				currentKeyBinding->first->second = key;
				currentKeyBinding->second->setText(virtualKeyCodeToString(key));
				inputManager->resetCurrentEditedKeyBinding();
			}
			inputManager->setKey(key, true);
		}
	}

	void Application::specialKeyInputUp(int key, int x, int y)
	{
		switch (key)
		{
			case GLUT_KEY_UP:
			{
				if (inputManager->getKey(VK_UP))
					inputManager->setKey(VK_UP, false);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				if (inputManager->getKey(VK_DOWN))
					inputManager->setKey(VK_DOWN, false);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				if (inputManager->getKey(VK_LEFT))
					inputManager->setKey(VK_LEFT, false);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				if (inputManager->getKey(VK_RIGHT))
					inputManager->setKey(VK_RIGHT, false);
				break;
			}
		}
	}

	void Application::specialKeyInput(int key, int x, int y)
	{
		int c = 0;
		std::string charText = "";
		switch (key)
		{
			case GLUT_KEY_UP:
			{
				charText = virtualKeyCodeToString(VK_UP);
				c = VK_UP;
				if (!inputManager->getKey(VK_UP))
					inputManager->setKey(VK_UP, true);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				charText = virtualKeyCodeToString(VK_DOWN);
				c = VK_DOWN;
				if (!inputManager->getKey(VK_DOWN))
					inputManager->setKey(VK_DOWN, true);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				charText = virtualKeyCodeToString(VK_LEFT);
				c = VK_LEFT;
				if (!inputManager->getKey(VK_LEFT))
					inputManager->setKey(VK_LEFT, true);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				charText = virtualKeyCodeToString(VK_RIGHT);
				c = VK_RIGHT;
				if (!inputManager->getKey(VK_RIGHT))
					inputManager->setKey(VK_RIGHT, true);
				break;
			}
		}

		auto keyBindings = inputManager->getKeyBindings();
		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		if (c != 0 && !std::any_of(keyBindings->begin(), keyBindings->end(), [c](keybinding element){return element.second == c; }) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentKeyBinding->first->second = c;
			currentKeyBinding->second->setText(charText);
			inputManager->resetCurrentEditedKeyBinding();
		}
	}

	void Application::motionFunc(int x, int y)
	{
		if (getState() != GameState::NOTSTARTEDYET && getState() != GameState::PAUSED) return;

		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		for (auto uiElement : currentMenu->second)
		{
			uiElement->checkIfMouseHoverThis(lastMousePosition);
		}
	}

	void Application::processMouseClick(int button, int state, int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (button == GLUT_LEFT_BUTTON)
		{
			inputManager->setKey(VK_LBUTTON, state == GLUT_DOWN ? true : false);
			inputManager->setLeftMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastLeftMouseState(state == GLUT_DOWN ? false : true);

			if (getState() != GameState::NOTSTARTEDYET && getState() != GameState::PAUSED) return;

			for (auto uiElement : currentMenu->second)
			{
				uiElement->checkForMouseClickOnThis(inputManager->getLeftMouseState(), inputManager->getLastLeftMouseState(), lastMousePosition);
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			inputManager->setKey(VK_RBUTTON, state == GLUT_DOWN ? true : false);
			inputManager->setRightMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastRightMouseState(state == GLUT_DOWN ? false : true);
		}
	}

	std::string Application::virtualKeyCodeToString(SHORT virtualKey)
	{
		UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		CHAR szName[128];
		int result = 0;
		switch (virtualKey)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= 0x100;
			default:
				result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}
		return szName;
	}

	std::shared_ptr<menu> Application::getMenu(std::string index)
	{
		for (auto _menu : gameMenu)
		{
			if (_menu->first == index)
				return _menu;
		}

		return nullptr;
	}
}