#include "Application.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include "rapidxml_print.hpp"
#include "Timer.h"

namespace Engine
{
	Application::Application() 
		: spriteSheetManager(std::make_shared<SpriteSheetManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), inputManager(std::make_shared<InputManager>()), gameState(GameState::NOTSTARTEDYET), characterSelectionIndex(0)
	{
		soundEngine = irrklang::createIrrKlangDevice();

		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;
		t = 0.0f;

		srand((int)time(NULL));

		onNotify = [this](ObserverEvent _event, std::vector<std::pair<std::string, BaseGameObject*>> params)
		{
			switch (_event)
			{
				case SCORE_CHANGED:
				{
					updatePlayerScore();
					break;
				}
				case LIVES_CHANGED:
				{
					updatePlayerLives();
					break;
				}
				case PLAYER_DIED:
				{
					player->setLives(player->getLives() - 1);
					if (player->getLives() < 1)
					{
						auto option = std::make_shared<Text>("Game Over", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
						option->setIsStatic(true);
						notifications.push_back(uiPlayerElement("Game Over", std::move(option)));

						player->clearBullets();
						inputManager->resetInput();
						gameState = GameState::ENDED;
					}

					player->setVelocity(player->getStartVelocity());
					player->setPosition(player->getStartPosition());
					break;
				}
				case OBJECT_DESTROYED:
				{
					auto collider = getParameter(params, "collider");
					auto explosionSound = params.begin()->first;

					player->setScore(player->getScore() + collider->getValue());

					auto explosion = std::make_shared<BaseGameObject>(0.0f, 0.0f, collider->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					explosion->applyAnimation(collider->getAnimationByIndex("explosion"));
					explosion->onUpdate = [explosion]()
					{
						explosion->changeColor(explosion->getColor(3) - 0.05f, 3);

						if (explosion->getColor(3) <= 0.0f)
						{
							explosion->setNeedsToBeRemoved(true);
						}
					};

					if (explosionSound.size())
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					}

					explosions.push_back(explosion);
					break;
				}
				case PICKUP_PICKED_UP:
				case BULLET_SHOT:
				{
					soundEngine->play2D(params.begin()->first.c_str(), GL_FALSE);
					break;
				}
			}
		};

		loadConfig();
		spriteSheetManager->loadSpriteSheetsFromConfig();
		loadPlayerModels();
		initGameUI();

		background = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
		background->applyAnimation(spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));
		background->setWidth((float)glutGet(GLUT_INIT_WINDOW_WIDTH));
		background->setHeight((float)glutGet(GLUT_INIT_WINDOW_HEIGHT));
	}

	Application::~Application()
	{
		enemies.clear();
		explosions.clear();
		notifications.clear();
		scoreBoard.clear();
		playerLives.clear();
		pickups.clear();
		meteors.clear();

		saveConfig();
	}

	void Application::updatePlayerLives()
	{
		playerLives.clear();

		auto option = std::make_shared<UIElementBase>(spriteSheetManager->getSpriteSheet("main")->getSprite(player->getLivesIcon())->getAnimation()->begin()->z, spriteSheetManager->getSpriteSheet("main")->getSprite(player->getLivesIcon())->getAnimation()->begin()->w, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(6.0f, 91.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(player->getLivesIcon()));
		playerLives.push_back(std::move(option));

		option = std::make_shared<UIElementBase>(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png")->getAnimation()->begin()->z, spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png")->getAnimation()->begin()->w, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f, 92.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png"));
		playerLives.push_back(std::move(option));

		option = std::make_shared<UIElementBase>(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getLives()) + ".png")->getAnimation()->begin()->z, spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getLives()) + ".png")->getAnimation()->begin()->w, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(16.0f, 92.0f));
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getLives()) + ".png"));
		playerLives.push_back(std::move(option));
	}

	void Application::updatePlayerScore()
	{
		scoreBoard.clear();

		auto str = std::to_string(player->getScore());

		for (size_t i = 0; i < str.size(); i++)
		{
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getLives()) + ".png")->getAnimation()->begin()->z, spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + std::to_string(player->getLives()) + ".png")->getAnimation()->begin()->w, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(70.0f + (4 * (i + 1)), 91.0f));
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			scoreBoard.push_back(std::move(option));
		}
	}

	void Application::resetScene()
	{
		explosions.clear();
		enemies.clear();
		pickups.clear();
		meteors.clear();
		notifications.clear();

		loadPlayerModel();

		currentLevel = 0;

		initScene();
		updatePlayerLives();
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
		auto option = std::make_shared<Text>("Start Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = getMenu("Character Selection");
		};
		mainMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("Options", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = getMenu("Options");
		};
		mainMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("End Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
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
		option = std::make_shared<Text>("Go To Main Menu", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

			currentMenu = getMenu("Main Menu");
			gameState = GameState::NOTSTARTEDYET;
		};
		pauseMenu->second.push_back(std::move(option));
		option = std::make_shared<Text>("End Game", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 55.0f));
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
			option = std::make_shared<Text>(it->first + " :", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
			option->setIsStatic(true);
			controls->second.push_back(std::move(option));
			option = std::make_shared<Text>(virtualKeyCodeToString(it->second), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
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
		option = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

			currentMenu = getMenu("Options");
		};
		controls->second.push_back(std::move(option));

		//Sounds
		option = std::make_shared<Text>("Volume :", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->setIsStatic(true);
		sounds->second.push_back(std::move(option));
		auto uniqueOption = std::make_shared<Text>(std::to_string((int)(soundEngine->getSoundVolume() * 100.0f)), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(75.0f, 60.0f));
		uniqueOption->setIsStatic(true);
		option = std::make_shared<Text>("<", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(70.0f, 60.0f));
		option->onMouseReleaseFunc = [this, uniqueOption]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			auto vol = soundEngine->getSoundVolume() - 0.01f;
			if (vol >= 0.0f)
			{
				soundEngine->setSoundVolume(vol);
				uniqueOption->setText(std::to_string((int)(vol * 100.f)));
			}
		};
		sounds->second.push_back(std::move(option));
		option = std::make_shared<Text>(">", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(85.0f, 60.0f));
		option->onMouseReleaseFunc = [this, uniqueOption]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);
			auto vol = soundEngine->getSoundVolume() + 0.01f;
			if (vol <= 1.0f)
			{
				soundEngine->setSoundVolume(vol);
				uniqueOption->setText(std::to_string((int)(vol * 100.f)));
			}
		};
		sounds->second.push_back(std::move(option));
		sounds->second.push_back(std::move(uniqueOption));
		option = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			
			currentMenu = getMenu("Options");
		};
		sounds->second.push_back(std::move(option));

		//Options
		option = std::make_shared<Text>("Controls", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			
			currentMenu = getMenu("Controls");
		};
		options->second.push_back(std::move(option));
		option = std::make_shared<Text>("Sounds", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			
			currentMenu = getMenu("Sounds");
		};
		options->second.push_back(std::move(option));
		option = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			
			currentMenu = getMenu("Main Menu");
		};
		options->second.push_back(std::move(option));

		//Character Selection
		option = std::make_shared<Text>("Pick your character :", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f));
		option->setIsStatic(true);
		characterSelection->second.push_back(std::move(option));
		auto option2 = std::make_shared<UIElementBase>(playerModels[characterSelectionIndex]->getAnimation()->begin()->z / 3.0f, playerModels[characterSelectionIndex]->getAnimation()->begin()->w / 3.0f, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(60.0f, 58.0f));
		option2->setIsStatic(true);
		option2->applyAnimation(playerModels[characterSelectionIndex]);
		option = std::make_shared<Text>("<", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		option->onMouseReleaseFunc = [this, option2]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			characterSelectionIndex--;

			if (characterSelectionIndex < 0) characterSelectionIndex = (playerModels.size() - 1);

			option2->applyAnimation(playerModels[characterSelectionIndex]);
		};
		characterSelection->second.push_back(std::move(option));
		option = std::make_shared<Text>(">", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(73.0f, 60.0f));
		option->onMouseReleaseFunc = [this, option2]()
		{
			soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

			characterSelectionIndex++;

			if (characterSelectionIndex > (playerModels.size() - 1)) characterSelectionIndex = 0;

			option2->applyAnimation(playerModels[characterSelectionIndex]);
		};
		characterSelection->second.push_back(std::move(option));
		characterSelection->second.push_back(std::move(option2));
		option = std::make_shared<Text>("Back", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 20.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

			currentMenu = getMenu("Main Menu");
		};
		characterSelection->second.push_back(std::move(option));
		option = std::make_shared<Text>("Start", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(29.0f, 20.0f));
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			resetScene();
			gameState = GameState::STARTED;
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

	void Application::initScene()
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
				for (auto beer_node = brewery_node->first_node("Enemy"); beer_node; beer_node = beer_node->next_sibling("Enemy"))
				{
					auto positionX = std::stof(beer_node->first_attribute("positionX")->value());
					auto positionY = std::stof(beer_node->first_attribute("positionY")->value());
					auto velocityX = std::stof(beer_node->first_attribute("velocityX")->value());
					auto velocityY = std::stof(beer_node->first_attribute("velocityY")->value());
					std::string spriteName = beer_node->first_attribute("spriteName")->value();
					auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);

					auto enemy = std::make_shared<Entity>(0.0f, 0.0f, glm::vec2(positionX, positionY), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					enemy->applyAnimation(sprite);
					enemy->setValue(std::stoi(beer_node->first_attribute("value")->value()));

					for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
					{
						std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
						std::vector<glm::vec4> bulletPositions;
						auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
						std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

						for (auto beer_node3 = beer_node2->first_node("Shoot"); beer_node3; beer_node3 = beer_node3->next_sibling("Shoot"))
						{
							bulletPositions.push_back(glm::vec4(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value()), std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())));
						}

						enemy->shootingMode = [bulletPositions, explosionSound, delayBetweenShoots, shootingSound, enemy, this]()
						{
							for (auto bulletPosition : bulletPositions)
							{
								auto bulletWidth = enemy->getAnimationByIndex("shoot")->getAnimation()->begin()->z;
								auto _bulletPosition = glm::vec2(enemy->getPosition(0) + (enemy->getWidth() / 2.0f) - (bulletWidth / 2.0f), enemy->getPosition(1) - (enemy->getHeight() / 2.0f));
								_bulletPosition += glm::vec2(enemy->getWidth() * bulletPosition.z, enemy->getHeight() * bulletPosition.w);

								enemy->addBullet(_bulletPosition, glm::vec2(bulletPosition.x, bulletPosition.y), explosionSound, shootingSound);
							}
						};
						enemy->setDelayBetweenShoots(delayBetweenShoots);
					}

					for (auto beer_node2 = beer_node->first_node("Animations"); beer_node2; beer_node2 = beer_node2->next_sibling("Animations"))
					{
						for (auto beer_node3 = beer_node2->first_node("Sprite"); beer_node3; beer_node3 = beer_node3->next_sibling("Sprite"))
						{
							enemy->addAnimation(beer_node3->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));
						}
						for (auto beer_node3 = beer_node2->first_node("Animation"); beer_node3; beer_node3 = beer_node3->next_sibling("Animation"))
						{
							enemy->addAnimation(beer_node3->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node3->first_attribute("animationName")->value()));
						}
					}

					enemy->onUpdate = [enemy, this]()
					{
						auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
						auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

						//Collision detection
						if (enemy->getPosition(0) + enemy->getWidth() >= windowWidth || enemy->getPosition(0) <= 0.0f)
							enemy->setVelocity(0, enemy->getVelocity(0) * -1.0f);

						if (enemy->getPosition(1) + enemy->getHeight() >= windowHeigth || enemy->getPosition(1) <= 0.0f)
							enemy->setVelocity(1, enemy->getVelocity(1) * -1.0f);
					};

					enemy->onCollision = [this, enemy](std::shared_ptr<BaseGameObject> collider)
					{
						auto entity = dynamic_cast<Entity*>(collider.get());

						enemy->setNeedsToBeRemoved(true);

						if (entity != nullptr)
						{
							if (entity->getAddon("shield") != nullptr)
							{
								entity->getAddon("shield")->setNeedsToBeRemoved(true);
							}
							else
							{
								auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
								params.push_back(std::pair<std::string, BaseGameObject*>("", nullptr));
								params.push_back(std::pair<std::string, BaseGameObject*>("collider", enemy.get()));
								onNotify(ObserverEvent::OBJECT_DESTROYED, params);

								entity->setNeedsToBeRemoved(true);
							}
						}
						else
						{
							auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
							params.push_back(std::pair<std::string, BaseGameObject*>("", nullptr));
							params.push_back(std::pair<std::string, BaseGameObject*>("collider", enemy.get()));
							onNotify(ObserverEvent::OBJECT_DESTROYED, params);

							collider->setNeedsToBeRemoved(true);
						}
					};
					enemy->addObserver(this);

					enemies.push_back(std::move(enemy));
				}

				for (auto beer_node = brewery_node->first_node("Meteor"); beer_node; beer_node = beer_node->next_sibling("Meteor"))
				{
					auto meteor = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					meteor->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
					meteor->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node->first_attribute("name")->value()));
					meteor->onUpdate = [meteor]()
					{
						meteor->setRotationAngle(meteor->getRotationAngle() + 0.05f);

						if(meteor->getRotationAngle() >= (3.14159265358979323846f * 2.0f))
							meteor->setRotationAngle(0.0f);
					};
					meteor->onCollision = [this, meteor](std::shared_ptr<BaseGameObject> collider)
					{
						auto entity = dynamic_cast<Entity*>(collider.get());

						meteor->setNeedsToBeRemoved(true);

						if (entity != nullptr)
						{
							if (entity->getAddon("shield") != nullptr)
							{
								entity->getAddon("shield")->setNeedsToBeRemoved(true);
							}
							else
							{
								auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
								params.push_back(std::pair<std::string, BaseGameObject*>("", nullptr));
								params.push_back(std::pair<std::string, BaseGameObject*>("collider", meteor.get()));
								onNotify(ObserverEvent::OBJECT_DESTROYED, params);

								entity->setNeedsToBeRemoved(true);
							}
						}
						else
						{
							auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
							params.push_back(std::pair<std::string, BaseGameObject*>("", nullptr));
							params.push_back(std::pair<std::string, BaseGameObject*>("collider", meteor.get()));
							onNotify(ObserverEvent::OBJECT_DESTROYED, params);

							collider->setNeedsToBeRemoved(true);
						}
					};
					meteor->addObserver(this);
					meteor->setValue(100);
					meteors.push_back(std::move(meteor));
				}

				for (auto beer_node = brewery_node->first_node("Pickup"); beer_node; beer_node = beer_node->next_sibling("Pickup"))
				{
					std::string pickupSound = beer_node->first_attribute("pickupSound")->value();
					std::string name = beer_node->first_attribute("name")->value();
					std::string spriteName = beer_node->first_attribute("spriteName")->value();
					auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
					auto pickup = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					pickup->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
					pickup->applyAnimation(sprite);

					for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
					{
						std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
						auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
						std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();
						std::vector<glm::vec4> bulletPositions;

						for (auto beer_node3 = beer_node2->first_node("Shoot"); beer_node3; beer_node3 = beer_node3->next_sibling("Shoot"))
						{
							bulletPositions.push_back(glm::vec4(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value()), std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())));
						}

						pickup->onCollision = [explosionSound, bulletPositions, delayBetweenShoots, shootingSound, pickupSound, pickup, this](std::shared_ptr<BaseGameObject> collider)
						{
							auto entity = dynamic_cast<Entity*>(collider.get());

							auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
							params.push_back(std::pair<std::string, BaseGameObject*>(pickupSound, nullptr));
							onNotify(ObserverEvent::PICKUP_PICKED_UP, params);

							pickup->setNeedsToBeRemoved(true);

							entity->shootingMode = [bulletPositions, shootingSound, explosionSound, delayBetweenShoots, entity, this]()
							{
								for (auto bulletPosition : bulletPositions)
								{
									auto bulletWidth = entity->getAnimationByIndex("shoot")->getAnimation()->begin()->z;
									auto _bulletPosition = glm::vec2(entity->getPosition(0) + (entity->getWidth() / 2.0f) - (bulletWidth / 2.0f), entity->getPosition(1) + (entity->getHeight() / 2.0f));
									_bulletPosition += glm::vec2(entity->getWidth() * bulletPosition.z, entity->getHeight() * bulletPosition.w);

									entity->addBullet(_bulletPosition, glm::vec2(bulletPosition.x, bulletPosition.y), explosionSound, shootingSound);
								}
							};
							
							entity->setDelayBetweenShoots(delayBetweenShoots);
							entity->setDelayBetweenShootsTimer(0.0f);
							return true;
						};
					}

					for (auto beer_node2 = beer_node->first_node("Shield"); beer_node2; beer_node2 = beer_node2->next_sibling("Shield"))
					{
						std::string spriteName = beer_node2->first_attribute("spriteName")->value();

						pickup->onCollision = [spriteName, pickup, pickupSound, this](std::shared_ptr<BaseGameObject> collider)
						{
							auto entity = dynamic_cast<Entity*>(collider.get());

							auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
							params.push_back(std::pair<std::string, BaseGameObject*>(pickupSound, nullptr));
							onNotify(ObserverEvent::PICKUP_PICKED_UP, params);

							pickup->setNeedsToBeRemoved(true);

							auto shield = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.7f));
							shield->setScale(0.5f);
							shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
							shield->onUpdate = [shield, entity]()
							{
								shield->changeColor(shield->getColor(3) - 0.05f, 3);

								if (shield->getColor(3) <= 0.0f)
								{
									shield->changeColor(0.7f, 3);
								}

								shield->setPosition(entity->getPosition() + glm::vec2(((shield->getWidth() - entity->getWidth()) * -1.0f) / 2.0f, ((shield->getHeight() - entity->getHeight()) * -1.0f) / 2.0f));
							};
							entity->addAddon(addon("shield", std::move(shield)));
							return true;
						};
					}

					pickups.push_back(std::move(pickup));
				}
			}
			i++;
		}
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
			std::string spriteName = brewery_node->first_attribute("spriteName")->value();

			playerModels.push_back(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
		}
	}

	void Application::loadPlayerModel()
	{
		size_t i = 0;
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
			if (i == characterSelectionIndex)
			{
				auto lives = std::stoi(brewery_node->first_attribute("lives")->value());
				std::string livesIcon = brewery_node->first_attribute("livesIcon")->value();
				std::string spriteName = brewery_node->first_attribute("spriteName")->value();
				auto velocityX = std::stof(brewery_node->first_attribute("velocityX")->value());
				auto velocityY = std::stof(brewery_node->first_attribute("velocityY")->value());
				auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);

				player = std::make_shared<Player>(0.0f, 0.0f, glm::vec2((float)glutGet(GLUT_WINDOW_WIDTH) / 2.0f, 0.0f), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				player->setLives(lives);
				player->applyAnimation(sprite);
				player->setLivesIcon(livesIcon);
				player->addObserver(this);

				for (auto beer_node = brewery_node->first_node("ShootingEffect"); beer_node; beer_node = beer_node->next_sibling("ShootingEffect"))
				{
					std::string explosionSound = beer_node->first_attribute("explosionSound")->value();
					std::vector<glm::vec4> bulletPositions;
					auto delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value());
					std::string shootingSound = beer_node->first_attribute("shootingSound")->value();

					for (auto beer_node2 = beer_node->first_node("Shoot"); beer_node2; beer_node2 = beer_node2->next_sibling("Shoot"))
					{
						bulletPositions.push_back(glm::vec4(std::stof(beer_node2->first_attribute("velocityX")->value()), std::stof(beer_node2->first_attribute("velocityY")->value()), std::stof(beer_node2->first_attribute("offsetX")->value()), std::stof(beer_node2->first_attribute("offsetY")->value())));
					}

					player->shootingMode = [bulletPositions, explosionSound, delayBetweenShoots, shootingSound, this]()
					{
						for (auto bulletPosition : bulletPositions)
						{
							auto bulletWidth = player->getAnimationByIndex("shoot")->getAnimation()->begin()->z;
							auto _bulletPosition = glm::vec2(player->getPosition(0) + (player->getWidth() / 2.0f) - (bulletWidth / 2.0f), player->getPosition(1) + (player->getHeight() / 2.0f));
							_bulletPosition += glm::vec2(player->getWidth() * bulletPosition.z, player->getHeight() * bulletPosition.w);

							player->addBullet(_bulletPosition, glm::vec2(bulletPosition.x, bulletPosition.y), explosionSound, shootingSound);
						}
					};
					player->setDelayBetweenShoots(delayBetweenShoots);
				}

				for (auto beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
				{
					for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
					{
						player->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));
					}
					for (auto beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
					{
						player->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node2->first_attribute("animationName")->value()));
					}
				}

				auto exhaust = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				exhaust->setScale(0.5f);
				exhaust->applyAnimation(player->getAnimationByIndex("noMovingExhaust"));
				exhaust->onUpdate = [exhaust, this]()
				{
					exhaust->setPosition(player->getPosition() + glm::vec2(0.0f, exhaust->getHeight() * -1.0f));
				};
				player->addAddon(addon("leftExhaust", std::move(exhaust)));

				exhaust = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				exhaust->setScale(0.5f);
				exhaust->applyAnimation(player->getAnimationByIndex("noMovingExhaust"));
				exhaust->onUpdate = [exhaust, this]()
				{
					exhaust->setPosition(player->getPosition() + glm::vec2(player->getWidth() - exhaust->getWidth(), exhaust->getHeight() * -1.0f));
				};
				player->addAddon(addon("rightExhaust", std::move(exhaust)));
			}
			i++;
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
			keyBindings2.push_back(keyBinding(it->first, std::to_string(it->second)));
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
		auto vol = std::to_string(soundEngine->getSoundVolume());
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
		initScene();
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
		auto frameTime = (newTime - currentTime) / 1000.0f;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			inputManager->fixInput();
			if (gameState == GameState::STARTED)
			{
				inputManager->updatePlayerInput(player, dt);
				player->update(dt);
			}

			if (gameState != GameState::NOTSTARTEDYET && gameState != GameState::PAUSED)
			{
				background->update(dt);

				for (auto it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt))
					{
						it = enemies.erase(it);
						if (enemies.size() == 0)
						{
							Timer::windowsTimer([this] { startNewLevel(); }, 2000);

							auto option = std::make_shared<Text>("Level completed!", glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
							option->setIsStatic(true);
							notifications.push_back(uiPlayerElement("Level completed", std::move(option)));
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

			if (gameState == GameState::STARTED)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
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

				for (auto it = meteors.begin(); it != meteors.end(); it++)
				{
					auto playerBulletList = player->getBulletsList();
					collisionManager->checkCollision(*it, playerBulletList, player);
				}

				collisionManager->checkCollision(player, &meteors);
				collisionManager->checkCollision(player, &enemies);
				collisionManager->checkCollision(player, &pickups);
			}

			if (gameState == GameState::STARTED)
				t += dt;

			accumulator -= dt;
		}

		if (gameState != GameState::NOTSTARTEDYET && gameState != GameState::PAUSED)
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
			for (auto it = enemies.begin(); it != enemies.end(); it++)
			{
				auto addons = (*it)->getAddons();
				(*it)->draw(renderer);
				for (auto it2 = addons->begin(); it2 != addons->end(); it2++)
				{
					it2->second->draw(renderer);
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
			for (auto it = enemies.begin(); it != enemies.end(); it++)
			{
				auto enemyBulletList = (*it)->getBulletsList();
				for (auto it2 = enemyBulletList->begin(); it2 != enemyBulletList->end(); it2++)
				{
					(*it2)->draw(renderer);
				}
			}

			auto playerBulletList = player->getBulletsList();
			for (auto it = playerBulletList->begin(); it != playerBulletList->end(); it++)
			{
				(*it)->draw(renderer);
			}
			//Render & Update player UI
			for (auto uiElement : playerLives)
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
		else if (gameState == GameState::NOTSTARTEDYET || gameState == GameState::PAUSED)
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
		//const auto ar = (float) width / (float) height;

		for (auto menu : gameMenu)
		{
			for (auto uiElement : menu->second)
			{
				uiElement->fixPosition();
			}
		}

		for (auto uiElement : playerLives)
			uiElement->fixPosition();

		for (auto uiElement : scoreBoard)
			uiElement->fixPosition();

		for (auto uiElement : notifications)
			uiElement.second->fixPosition();

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

	void Application::keyboardInputUp(unsigned char c, glm::vec2 position)
	{
		auto key = VkKeyScan(c);
		if (inputManager->getKey(key))
			inputManager->setKey(key, false);
	}

	void Application::keyboardInput(unsigned char c, glm::vec2 position)
	{
		auto key = VkKeyScan(c);
		if (!inputManager->getKey(key))
		{
			switch (c)
			{
				case 27:
				{
					if (gameState == GameState::ENDED)
					{
						currentMenu = getMenu("Main Menu");
						gameState = GameState::NOTSTARTEDYET;
					}
					else if (gameState == GameState::PAUSED)
					{
						gameState = GameState::STARTED;
					}
					else if (gameState == GameState::STARTED)
					{
						currentMenu = getMenu("Pause Menu");
						gameState = GameState::PAUSED;
					}
					else if (gameState == GameState::NOTSTARTEDYET)
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

	void Application::specialKeyInputUp(int key, glm::vec2 position)
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

	void Application::specialKeyInput(int key, glm::vec2 position)
	{
		auto c = 0;
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

	void Application::motionFunc(glm::vec2 position)
	{
		if (gameState == GameState::NOTSTARTEDYET || gameState == GameState::PAUSED)
		{
			for (auto uiElement : currentMenu->second)
			{
				uiElement->checkIfMouseHoverThis(position);
			}
		}
	}

	void Application::processMouseClick(int button, int state, glm::vec2 position)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			inputManager->setKey(VK_LBUTTON, state == GLUT_DOWN ? true : false);
			inputManager->setLeftMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastLeftMouseState(state == GLUT_DOWN ? false : true);

			if (gameState == GameState::NOTSTARTEDYET || gameState == GameState::PAUSED)
			{
				for (auto uiElement : currentMenu->second)
				{
					uiElement->checkForMouseClickOnThis(inputManager->getLeftMouseState(), inputManager->getLastLeftMouseState(), position);
				}
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
		auto result = 0;
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