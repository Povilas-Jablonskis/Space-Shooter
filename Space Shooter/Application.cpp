#include "Application.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include "rapidxml_print.hpp"

namespace Engine
{
	Application::~Application()
	{
		savePlayerConfig();
	}

	Application::Application()
		: spriteSheetManager(std::make_shared<SpriteSheetManager>()), menuManager(std::make_shared<MenuManager>()), configurationManager(std::make_shared<ConfigurationManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), inputManager(std::make_shared<InputManager>()), gameState(GameState::IN_MENU), lastGameState(GameState::IN_MENU), characterSelectionIndex(0)
	{
		soundEngine = irrklang::createIrrKlangDevice();

		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;

		srand((int)time(NULL));

		renderer->addShader("shader", std::move(std::make_shared<Shader>("shader.vert", "shader.frag")));
		renderer->addShader("textshader", std::move(std::make_shared<Shader>("textshader.vert", "textshader.frag")));

		loadPlayerConfig();
		spriteSheetManager->loadSpriteSheetsFromConfig();
		loadPlayerModels();
		initGameMenu();
	}

	void Application::onNotify(ObserverEvent _event)
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
				auto option = std::make_shared<Text>("Game Over", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(50.0f, 55.0f), std::make_shared<UIInputComponent>());
				option->disable();
				notifications.push_back(uiPlayerElement("Game Over", std::move(option)));

				setGameState(GameState::ENDED);
				break;
			}
			case GAMESTATE_CHANGED:
			{
				if (getGameState() == GameState::ENDED || getGameState() == GameState::IN_MENU || getGameState() == GameState::IN_PAUSED_MENU)
				{
					glutSetCursor(GLUT_CURSOR_INHERIT);
				}
				else if (getGameState() == GameState::STARTED)
				{
					glutSetCursor(GLUT_CURSOR_NONE);
				}
				break;
			}
		}
	}

	void Application::onNotify(ObserverEvent _event, BaseGameObject* obj)
	{
		switch (_event)
		{
			case OBJECT_DESTROYED:
			{
				player->setScore(player->getScore() + obj->getValue());

				if (obj->getExplosionSound().size())
				{
					soundEngine->play2D(obj->getExplosionSound().c_str(), GL_FALSE);
				}

				if (obj->getAnimationByIndex("explosion") == nullptr) return;

				auto explosion = std::make_shared<BaseGameObject>(obj->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				explosion->applyAnimation(obj->getAnimationByIndex("explosion"));
				explosion->onUpdateFunc = [explosion]()
				{
					explosion->changeColor(explosion->getColor().a - 0.05f, 3);

					if (explosion->getColor().a <= 0.0f)
					{
						explosion->setNeedsToBeRemoved(true);
					}
				};

				explosions.push_back(explosion);
				break;
			}
		}
	}

	void Application::updatePlayerLives()
	{
		playerLives.clear();

		auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(6.0f, 91.0f), std::make_shared<UIInputComponent>());
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(player->getLivesIcon()));
		playerLives.push_back(std::move(option));

		option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f, 91.0f), std::make_shared<UIInputComponent>());
		option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeralX.png"));
		playerLives.push_back(std::move(option));

		auto str = std::to_string(player->getLives());

		for (size_t i = 0; i < str.size(); i++)
		{
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(12.0f + (4 * (i + 1)), 91.0f), std::make_shared<UIInputComponent>());
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			playerLives.push_back(std::move(option));
		}
	}

	void Application::updatePlayerScore()
	{
		scoreBoard.clear();

		auto str = std::to_string(player->getScore());

		for (size_t i = 0; i < str.size(); i++)
		{
			std::string str2(1, str[i]);
			auto option = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(70.0f + (4 * (i + 1)), 91.0f), std::make_shared<UIInputComponent>());
			option->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("numeral" + str2 + ".png"));
			scoreBoard.push_back(std::move(option));
		}
	}

	void Application::initGameMenu()
	{
		menuManager->getMenus()->clear();

		auto mainMenu = std::make_shared<menu>("Main Menu", std::make_shared<Menu>());

		//Main Menu
		auto option = std::make_shared<Text>("Start Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f), std::make_shared<UIInputComponent>());
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			auto characterSelection = std::make_shared<menu>("Character Selection", std::make_shared<Menu>());

			//Character Selection
			auto option = std::make_shared<Text>("Pick your character :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f), std::make_shared<UIInputComponent>());
			option->disable();
			characterSelection->second->addText(std::move(option));
			auto option2 = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(43.0f, 60.0f), std::make_shared<UIInputComponent>());
			option2->applyAnimation(playerModels[characterSelectionIndex]);
			option2->setScale(0.5f);
			option = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(40.0f, 60.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this, option2]()
			{
				soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

				characterSelectionIndex--;

				if (characterSelectionIndex < 0) characterSelectionIndex = (playerModels.size() - 1);

				option2->applyAnimation(playerModels[characterSelectionIndex]);
			};
			characterSelection->second->addText(std::move(option));
			option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(49.0f, 60.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this, option2]()
			{
				soundEngine->play2D("Sounds/buttonselect/1.wav", GL_FALSE);

				characterSelectionIndex++;

				if (characterSelectionIndex > (playerModels.size() - 1)) characterSelectionIndex = 0;

				option2->applyAnimation(playerModels[characterSelectionIndex]);
			};
			characterSelection->second->addText(std::move(option));
			characterSelection->second->addUIElement(std::move(option2));
			option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 20.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this]()
			{
				soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

				menuManager->getMenus()->pop_back();
			};
			characterSelection->second->addText(std::move(option));
			option = std::make_shared<Text>("Start", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(25.0f, 20.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				currentLevel = 0;
				accumulator = 0.0f;

				initLevel();
				loadPlayerModel();

				setGameState(GameState::STARTED);
			};
			characterSelection->second->addText(std::move(option));

			menuManager->addMenu(std::move(characterSelection));
		};
		mainMenu->second->addText(std::move(option));
		option = std::make_shared<Text>("Options", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f), std::make_shared<UIInputComponent>());
		option->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			auto options = std::make_shared<menu>("Options", std::make_shared<Menu>());

			//Options
			auto option = std::make_shared<Text>("Controls", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				auto controls = std::make_shared<menu>("Controls", std::make_shared<Menu>());

				//Controls
				auto keybindings = inputManager->getKeyBindings();
				float i = 0.0f;
				for (auto keybinding : keybindings)
				{
					auto option = std::make_shared<Text>(keybinding.first + " :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f - (3.0f * i)), std::make_shared<UIInputComponent>());
					option->disable();
					controls->second->addText(std::move(option));
					option = std::make_shared<Text>(KeyBindingInputComponent::virtualKeyCodeToString(keybinding.second), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(29.0f, 60.0f - (3.0f * i)), std::make_shared<UIInputComponent>(), std::make_shared<KeyBindingInputComponent>(keybinding.first));
					option->onMouseReleaseFunc = [this, option, keybinding]()
					{
						if (inputManager->getCurrentlyEditedKeyBinding().empty())
						{
							soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

							inputManager->setCurrentlyEditedKeyBinding(keybinding.first);

							option->disable();
							option->onHoverEnterFuncDefaults();
						}
					};
					controls->second->addText(std::move(option));
					i += 1.0f;
				}
				auto option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f), std::make_shared<UIInputComponent>());
				option->onMouseReleaseFunc = [this]()
				{
					soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

					menuManager->getMenus()->pop_back();
				};
				controls->second->addText(std::move(option));

				menuManager->addMenu(std::move(controls));
			};
			options->second->addText(std::move(option));
			option = std::make_shared<Text>("Sounds", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this]()
			{
				soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

				auto sounds = std::make_shared<menu>("Sounds", std::make_shared<Menu>());

				//Sounds
				auto option = std::make_shared<Text>("Volume :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f), std::make_shared<UIInputComponent>());
				option->disable();
				sounds->second->addText(std::move(option));
				auto uniqueOption = std::make_shared<Text>(std::to_string((int)(soundEngine->getSoundVolume() * 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(32.0f, 60.0f), std::make_shared<UIInputComponent>());
				uniqueOption->disable();
				option = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(29.0f, 60.0f), std::make_shared<UIInputComponent>());
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
				sounds->second->addText(std::move(option));
				option = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(36.0f, 60.0f), std::make_shared<UIInputComponent>());
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
				sounds->second->addText(std::move(option));
				sounds->second->addText(std::move(uniqueOption));
				option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 20.0f), std::make_shared<UIInputComponent>());
				option->onMouseReleaseFunc = [this]()
				{
					soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

					menuManager->getMenus()->pop_back();
				};
				sounds->second->addText(std::move(option));

				menuManager->addMenu(std::move(sounds));
			};
			options->second->addText(std::move(option));
			option = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 54.0f), std::make_shared<UIInputComponent>());
			option->onMouseReleaseFunc = [this]()
			{
				soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);

				menuManager->getMenus()->pop_back();
			};
			options->second->addText(std::move(option));

			menuManager->addMenu(std::move(options));
		};
		mainMenu->second->addText(std::move(option));
		option = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 54.0f), std::make_shared<UIInputComponent>());
		option->onMouseReleaseFunc = [this]()
		{
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		mainMenu->second->addText(std::move(option));

		menuManager->addMenu(std::move(mainMenu));
	}

	void Application::initLevel()
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
				menuManager->getMenus()->clear();
				explosions.clear();
				enemies.clear();
				pickups.clear();
				meteors.clear();
				notifications.clear();

				background.reset();
				background = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(0.0f, 0.0f), std::make_shared<UIInputComponent>());
				background->applyAnimation(spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));

				for (auto beer_node = brewery_node->first_node("Enemy"); beer_node; beer_node = beer_node->next_sibling("Enemy"))
				{
					auto positionX = std::stof(beer_node->first_attribute("positionX")->value());
					auto positionY = std::stof(beer_node->first_attribute("positionY")->value());
					auto velocityX = std::stof(beer_node->first_attribute("velocityX")->value());
					auto velocityY = std::stof(beer_node->first_attribute("velocityY")->value());
					std::string spriteName = beer_node->first_attribute("spriteName")->value();
					auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);

					auto enemy = std::make_shared<Entity>(glm::vec2(positionX, positionY), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					enemy->addObserver(this);
					enemy->applyAnimation(sprite);
					enemy->setValue(std::stoi(beer_node->first_attribute("value")->value()));

					for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
					{
						std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
						std::vector<std::shared_ptr<BaseGameObject>> bullets;
						auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
						std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

						for (auto beer_node3 = beer_node2->first_node("Bullet"); beer_node3; beer_node3 = beer_node3->next_sibling("Bullet"))
						{
							auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
							bullet->addObserver(this);
							bullet->setExplosionSound(explosionSound);
							bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

							for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
							{
								for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
								{
									bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
								}
								for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
								{
									bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
								}
							}

							bullets.push_back(std::move(bullet));
						}

						enemy->shootingModeFunc = [bullets, shootingSound, enemy, this]()
						{
							for (auto bullet : bullets)
							{
								auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
								auto _bulletPosition = glm::vec2(enemy->getPosition().x + (enemy->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), enemy->getPosition().y - (enemy->getHeight() / 2.0f));
								_bulletPosition += glm::vec2(enemy->getWidth() * _bullet->getPosition().x, enemy->getHeight() * _bullet->getPosition().y);
								_bullet->setPosition(_bulletPosition);
								enemy->addBullet(_bullet);
							}

							soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
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

					enemy->onUpdateFunc = [enemy, this]()
					{
						auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
						auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

						//Collision detection
						if (enemy->getPosition().x + enemy->getWidth() >= windowWidth || enemy->getPosition().x <= 0.0f)
							enemy->setVelocity(0, enemy->getVelocity().x * -1.0f);

						if (enemy->getPosition().y + enemy->getHeight() >= windowHeigth || enemy->getPosition().y <= 0.0f)
							enemy->setVelocity(1, enemy->getVelocity().y * -1.0f);
					};

					enemies.push_back(std::move(enemy));
				}

				for (auto beer_node = brewery_node->first_node("Meteor"); beer_node; beer_node = beer_node->next_sibling("Meteor"))
				{
					auto meteor = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					meteor->addObserver(this);
					meteor->setExplosionSound(beer_node->first_attribute("explosionSound")->value());
					meteor->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
					meteor->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node->first_attribute("spriteName")->value()));
					meteor->onUpdateFunc = [meteor]()
					{
						meteor->setRotationAngle(meteor->getRotationAngle() + 0.05f);

						if(meteor->getRotationAngle() >= (3.14159265358979323846f * 2.0f))
							meteor->setRotationAngle(0.0f);
					};

					meteor->setValue(100);
					meteors.push_back(std::move(meteor));
				}

				for (auto beer_node = brewery_node->first_node("Pickup"); beer_node; beer_node = beer_node->next_sibling("Pickup"))
				{
					std::string spriteName = beer_node->first_attribute("spriteName")->value();
					auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
					auto pickup = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					pickup->setExplosionSound(beer_node->first_attribute("pickupSound")->value());
					pickup->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
					pickup->applyAnimation(sprite);

					for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
					{
						std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
						std::vector<std::shared_ptr<BaseGameObject>> bullets;
						auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
						std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

						for (auto beer_node3 = beer_node2->first_node("Bullet"); beer_node3; beer_node3 = beer_node3->next_sibling("Bullet"))
						{
							auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
							bullet->addObserver(this);
							bullet->setExplosionSound(explosionSound);
							bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

							for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
							{
								for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
								{
									bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
								}
								for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
								{
									bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
								}
							}

							bullets.push_back(std::move(bullet));
						}

						pickup->onCollisionFunc = [bullets, shootingSound, delayBetweenShoots, pickup, this](std::shared_ptr<BaseGameObject> collider)
						{
							auto entity = dynamic_cast<Entity*>(collider.get());

							pickup->setNeedsToBeRemoved(true);

							entity->shootingModeFunc = [bullets, shootingSound, entity, this]()
							{
								for (auto bullet : bullets)
								{
									auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
									auto _bulletPosition = glm::vec2(entity->getPosition().x + (entity->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), entity->getPosition().y + (entity->getHeight() / 2.0f));
									_bulletPosition += glm::vec2(entity->getWidth() * _bullet->getPosition().x, entity->getHeight() * _bullet->getPosition().y);
									_bullet->setPosition(_bulletPosition);

									entity->addBullet(_bullet);
								}

								soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
							};
							
							entity->setDelayBetweenShoots(delayBetweenShoots);
							entity->setDelayBetweenShootsTimer(0.0f);
							return true;
						};
					}

					for (auto beer_node2 = beer_node->first_node("Shield"); beer_node2; beer_node2 = beer_node2->next_sibling("Shield"))
					{
						std::string spriteName = beer_node2->first_attribute("spriteName")->value();

						pickup->onCollisionFunc = [spriteName, pickup, this](std::shared_ptr<BaseGameObject> collider)
						{
							auto entity = dynamic_cast<Entity*>(collider.get());

							pickup->setNeedsToBeRemoved(true);

							auto shield = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.7f));

							shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
							shield->onUpdateFunc = [shield, entity]()
							{
								shield->changeColor(shield->getColor().a - 0.05f, 3);

								if (shield->getColor().a <= 0.0f)
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

		theFile.close();
		doc.clear();
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

		theFile.close();
		doc.clear();
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

				player = std::make_shared<Player>(glm::vec2((float)glutGet(GLUT_WINDOW_WIDTH) / 2.0f, 0.0f), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), std::make_shared<InputComponent>());
				player->addObserver(this);
				player->setLivesIcon(livesIcon);
				player->setScore(0);
				player->setLives(lives);
				player->applyAnimation(sprite);

				for (auto beer_node = brewery_node->first_node("ShootingEffect"); beer_node; beer_node = beer_node->next_sibling("ShootingEffect"))
				{
					std::string explosionSound = beer_node->first_attribute("explosionSound")->value();
					std::vector<std::shared_ptr<BaseGameObject>> bullets;
					auto delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value());
					std::string shootingSound = beer_node->first_attribute("shootingSound")->value();

					for (auto beer_node2 = beer_node->first_node("Bullet"); beer_node2; beer_node2 = beer_node2->next_sibling("Bullet"))
					{
						auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node2->first_attribute("offsetX")->value()), std::stof(beer_node2->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node2->first_attribute("velocityX")->value()), std::stof(beer_node2->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
						bullet->addObserver(this);
						bullet->setExplosionSound(explosionSound);
						bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));

						for (auto beer_node3 = beer_node2->first_node("Animations"); beer_node3; beer_node3 = beer_node3->next_sibling("Animations"))
						{
							for (auto beer_node4 = beer_node3->first_node("Sprite"); beer_node4; beer_node4 = beer_node4->next_sibling("Sprite"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node4->first_attribute("spriteName")->value()));
							}
							for (auto beer_node4 = beer_node3->first_node("Animation"); beer_node4; beer_node4 = beer_node4->next_sibling("Animation"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node4->first_attribute("animationName")->value()));
							}
						}

						bullets.push_back(std::move(bullet));
					}

					player->shootingModeFunc = [bullets, shootingSound, this]()
					{
						for (auto bullet : bullets)
						{
							auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
							auto _bulletPosition = glm::vec2(player->getPosition().x + (player->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), player->getPosition().y + (player->getHeight() / 2.0f));
							_bulletPosition += glm::vec2(player->getWidth() * _bullet->getPosition().x, player->getHeight() * _bullet->getPosition().y);
							_bullet->setPosition(_bulletPosition);

							player->addBullet(_bullet);
						}

						soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
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
			}
			i++;
		}

		theFile.close();
		doc.clear();
	}

	void Application::savePlayerConfig()
	{
		typedef std::pair<std::string, std::string> keyBinding;

		auto keyBindings = inputManager->getKeyBindings();
		rapidxml::xml_document<> doc;

		rapidxml::xml_node<>* Config = doc.allocate_node(rapidxml::node_element, "Config");

		doc.append_node(Config);

		rapidxml::xml_node<>* KeyBindings = doc.allocate_node(rapidxml::node_element, "KeyBindings");

		for (auto keyBinding : keyBindings)
		{
			rapidxml::xml_node<>* KeyBinding = doc.allocate_node(rapidxml::node_element, "KeyBinding");
			auto attribute_value = doc.allocate_string(keyBinding.first.c_str());
			KeyBinding->append_attribute(doc.allocate_attribute("key", attribute_value));
			attribute_value = doc.allocate_string(std::to_string(keyBinding.second).c_str());
			KeyBinding->append_attribute(doc.allocate_attribute("value", attribute_value));
			KeyBindings->append_node(KeyBinding);
		}

		Config->append_node(KeyBindings);

		rapidxml::xml_node<>* Volume = doc.allocate_node(rapidxml::node_element, "Volume");
		auto attribute_value = doc.allocate_string(std::to_string(soundEngine->getSoundVolume()).c_str());
		Volume->append_attribute(doc.allocate_attribute("value", attribute_value));
		Config->append_node(Volume);

		std::ofstream file_stored("Config/playerConfig.xml");
		file_stored << doc;
		file_stored.close();
		doc.clear();
	}

	void Application::loadPlayerConfig()
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/playerConfig.xml");
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
				inputManager->setKeyBinding(std::pair<std::string, int>(beer_node->first_attribute("key")->value(), std::stoi(beer_node->first_attribute("value")->value())));
			}
		}

		for (auto brewery_node = root_node->first_node("Volume"); brewery_node; brewery_node = brewery_node->next_sibling("Volume"))
		{
			soundEngine->setSoundVolume(std::stof(brewery_node->first_attribute("value")->value()));
		}

		theFile.close();
		doc.clear();
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
			if (getGameState() != GameState::IN_MENU && getGameState() != GameState::IN_PAUSED_MENU)
			{
				player->update(dt, inputManager);

				for (auto it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt))
						it = enemies.erase(it);
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

				if (enemies.size() == 0)
				{
					currentLevel++;
					initLevel();
				}
			}

			if (getGameState() == GameState::STARTED)
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

			accumulator -= dt;
		}

		if (getGameState() != GameState::IN_MENU && getGameState() != GameState::IN_PAUSED_MENU)
		{
			//Render background
			background->update(dt, inputManager);
			renderer->draw(std::vector<std::shared_ptr<UIElementBase>>{background});
			//Render meteors
			renderer->draw(meteors);
			//Render player & his addons
			renderer->draw(std::vector<std::shared_ptr<Player>>{player});
			renderer->draw(*player->getAddons());
			//Render enemies & their addons
			renderer->draw(enemies);
			for (auto enemy : enemies)
			{
				renderer->draw(*enemy->getAddons());
			}
			//Render pickups
			renderer->draw(pickups);
			//Render explosions
			renderer->draw(explosions);
			//Render enemies bullets
			for (auto it = enemies.begin(); it != enemies.end(); it++)
			{
				renderer->draw(*(*it)->getBulletsList());
			}
			//Render player's bullets
			renderer->draw(*player->getBulletsList());
			//Render & Update UI elements
			for (auto uiElement : playerLives)
			{
				uiElement->update(dt, inputManager);
			}

			renderer->draw(playerLives);

			for (auto uiElement : scoreBoard)
			{
				uiElement->update(dt, inputManager);
			}

			renderer->draw(scoreBoard);

			for (auto uiElement : notifications)
			{
				uiElement.second->update(dt, configurationManager, inputManager);
			}

			renderer->draw(notifications);
		}
		
		if (getGameState() == GameState::IN_MENU || getGameState() == GameState::IN_PAUSED_MENU)
		{
			auto menus = menuManager->getMenus();

			auto back = menus->back()->second;
			auto uiElements = back->getElements();
			auto texts = back->getTexts();

			for (auto text : texts)
			{
				text->update(dt, configurationManager, inputManager);
			}

			for (auto uiElement : uiElements)
			{
				uiElement->update(dt, inputManager);
			}

			renderer->draw(back);
		}

		inputManager->update();

		glutSwapBuffers();
	}
}