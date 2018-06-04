#include "Application.h"
#include <ctime>

namespace Engine
{
	Application::Application() 
		: effectManager(std::make_shared<EffectManager>()), enemyManager(std::make_shared<EnemyManager>()), pickupManager(std::make_shared<PickupManager>()), spriteSheetManager(std::make_shared<SpriteSheetManager>()), collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), inputManager(std::make_shared<InputManager>()), gameState(GameState::NOTSTARTEDYET)
	{
		soundEngine = irrklang::createIrrKlangDevice();

		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;
		t = 0.0f;

		srand((int)time(NULL));

		onNotify = [this](ObserverEvent _event, BaseGameObject* subject)
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
					soundEngine->play2D("Sounds/explosions/6.wav", GL_FALSE);

					auto explosion = std::make_shared<Explosion>(32.0f, 32.0f, subject->getPosition());
					explosion->applyAnimation(subject->getAnimationByIndex("explosion"));
					addExplosionToList(std::move(explosion));
					break;
				}
				case BULLETSHOT:
				{
					auto _subject = dynamic_cast<Entity*>(subject);
					if (_subject != nullptr)
						soundEngine->play2D(_subject->getShootingSound().c_str(), GL_FALSE);
					break;
				}
			}
		};

		inputManager->setKeyBinding("Attack", VK_SPACE);
		inputManager->setKeyBinding("Move Left", 0x41);
		inputManager->setKeyBinding("Move Right", 0x44);
		inputManager->setKeyBinding("Move Back", 0x53);

		renderer->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		renderer->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		fontManager->loadFont("kenvector_future.ttf", "kenvector_future");
		fontManager->loadFont("kenvector_future_thin.ttf", "kenvector_future_thin");

		initSpriteSheets();
		effectManager->loadEffectsFromConfig(spriteSheetManager, soundEngine);
		pickupManager->loadPickupsFromConfig(spriteSheetManager, effectManager);
		enemyManager->loadEnemiesFromConfig(spriteSheetManager, effectManager);

		background = std::make_shared<UIElementBase>((float)glutGet(GLUT_INIT_WINDOW_WIDTH), (float)glutGet(GLUT_INIT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
		background->applyAnimation(spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));

		initGameUI();
		currentMenu = getUIElement("Main Menu");
	}

	Application::~Application()
	{
		enemies.clear();
		explosions.clear();
		ui.clear();
		playerUI.clear();
		pickups.clear();
		meteors.clear();
	}

	void Application::initSpriteSheets()
	{
		auto spriteSheet = std::make_shared<SpriteSheet>();
		auto backgroundSpriteSheet = std::make_shared<SpriteSheet>();

		backgroundSpriteSheet->loadSpriteSheet("Backgrounds/blue.png");

		spriteSheet->loadSpriteSheet("Spritesheet/sheet.png");
		spriteSheet->loadSpritesFromXml("Spritesheet/sheet.xml");

		std::vector<glm::vec4> sprites;
		sprites.push_back(spriteSheet->getSpriteAsVector("laserBlue11.png"));
		sprites.push_back(spriteSheet->getSpriteAsVector("laserBlue10.png"));
		spriteSheet->makeAnimation("blueExplosionSpriteSheet", sprites);

		sprites.clear();

		sprites.push_back(spriteSheet->getSpriteAsVector("laserGreen01.png"));
		sprites.push_back(spriteSheet->getSpriteAsVector("laserGreen16.png"));
		spriteSheet->makeAnimation("greenExplosionSpriteSheet", sprites);

		sprites.clear();

		sprites.push_back(spriteSheet->getSpriteAsVector("shield1.png"));
		sprites.push_back(spriteSheet->getSpriteAsVector("shield2.png"));
		sprites.push_back(spriteSheet->getSpriteAsVector("shield3.png"));
		spriteSheet->makeAnimation("shieldSpriteSheet", sprites);

		spriteSheet->getAnimation("shieldSpriteSheet")->setLoopStatus(true);
		spriteSheet->getAnimation("shieldSpriteSheet")->setDelay(0.15f);

		sprites.clear();

		sprites.push_back(spriteSheet->getSpriteAsVector("meteorBrown_med1.png"));
		sprites.push_back(spriteSheet->getSpriteAsVector("meteorBrown_med3.png"));
		spriteSheet->makeAnimation("meteorBrown_med", sprites);

		spriteSheet->getAnimation("meteorBrown_med")->setLoopStatus(true);
		spriteSheet->getAnimation("meteorBrown_med")->setDelay(0.15f);

		spriteSheetManager->loadSpriteSheet("main", spriteSheet);
		spriteSheetManager->loadSpriteSheet("background", backgroundSpriteSheet);
	}

	void Application::removeEnemyFromList(std::vector<std::shared_ptr<Enemy>>::iterator* it)
	{
		if (*it != enemies.end())
			*it = enemies.erase(*it);

		if (enemies.size() == 0)
		{
			Timer::windowsTimer([this]{ startNewLevel(); }, 2000);
			getPlayerUIElement("Level completed")->showMain(false);
		}
	}

	void Application::removeExplosionFromList(std::vector<std::shared_ptr<Explosion>>::iterator* it)
	{
		if (*it != explosions.end())
			*it = explosions.erase(*it);
	}

	void Application::initPlayerUI()
	{
		playerUI.clear();

		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Score")->addText(std::move(option));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option2 = std::make_shared<UIElement>(33.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("playerLife1_blue.png"));
		getPlayerUIElement("Health")->addUIElement(std::move(option2));
		option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Health")->addText(std::move(option));

		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option3 = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		option3->setIsStatic(true);
		getPlayerUIElement("Level completed")->addText(std::move(option3));

		getPlayerUIElement("Score")->fixPosition();
		getPlayerUIElement("Health")->fixPosition();
		getPlayerUIElement("Level completed")->fixPosition();
	}

	void Application::updatePlayerHealth()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		erasePlayerUIElement("Health");
		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		//Health
		auto option2 = std::make_shared<UIElement>(33.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("playerLife1_blue.png"));
		getPlayerUIElement("Health")->addUIElement(std::move(option2));
		auto option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Health")->addText(std::move(option));
		getPlayerUIElement("Health")->fixPosition();
	}

	void Application::updatePlayerScore()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		erasePlayerUIElement("Score");
		playerUI.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		//Score
		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		getPlayerUIElement("Score")->addText(std::move(option));
		getPlayerUIElement("Score")->fixPosition();
	}

	void Application::initScene()
	{
		player = std::make_shared<Player>(32.0f, 32.0f, glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f), glm::vec2(80.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		player->onDeath = [this]()
		{
			inputManager->resetInput();
			setState(GameState::ENDED);
			currentMenu = getUIElement("Game Over");
			getUIElement("Game Over")->showMain(false);
		};
		player->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("playerShip1_blue.png"));
		player->addAnimation("shoot", spriteSheetManager->getSpriteSheet("main")->getSprite("laserBlue01.png"));
		player->addAnimation("explosion", spriteSheetManager->getSpriteSheet("main")->getAnimation("blueExplosionSpriteSheet"));
		player->addObserver(this);

		enemies.clear();
		float space = 100.f;
		size_t max = rand() % 10 + 1;
		for (size_t i = 0; enemies.size() < max; i++)
		{
			auto found = false;
			auto enemy = enemyManager->getRandomEnemy();
			auto randX = randomFloat(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH) - 32.0f);
			auto randY = randomFloat(340.0f, (float)glutGet(GLUT_WINDOW_HEIGHT) - 32.0f);
			enemy->setPosition(0, randX);
			enemy->setPosition(1, randY);

			if (collisionManager->checkCollision(player, enemy))
				found = true;

			for (auto enemyForCollision : enemies)
			{
				if (collisionManager->checkCollision(enemy, enemyForCollision))
				{
					found = true;
					break;
				}
			}

			if (found)
				continue;

			enemy->addObserver(this);
			enemies.push_back(std::move(enemy));
		}

		pickups.clear();
		auto pickup = pickupManager->getRandomPickup();
		pickup->setPosition(glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, (float)glutGet(GLUT_WINDOW_Y) / 2.0f));
		pickups.push_back(std::move(pickup));

		meteors.clear();
		for (size_t i = 0; meteors.size() < 10; i++)
		{
			auto found = false;
			auto meteor = std::make_shared<BaseGameObject>(32.0f, 32.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			auto randX = randomFloat(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH) - 32.0f);
			auto randY = randomFloat(100.0f, 300.0f);
			meteor->setPosition(0, randX);
			meteor->setPosition(1, randY);

			if (collisionManager->checkCollision(player, meteor))
				found = true;

			for (auto enemy : enemies)
			{
				if (collisionManager->checkCollision(enemy, meteor))
				{
					found = true;
					break;
				}
			}

			for (auto meteorForCollision : meteors)
			{
				if (collisionManager->checkCollision(meteorForCollision, meteor))
				{
					found = true;
					break;
				}
			}

			for (auto pickup : pickups)
			{
				if (collisionManager->checkCollision(pickup, meteor))
				{
					found = true;
					break;
				}
			}

			if (found)
				continue;

			meteor->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getAnimation("meteorBrown_med"));
			meteor->onCollision = [this, meteor](std::shared_ptr<BaseGameObject> collider)
			{
				soundEngine->play2D("Sounds/explosions/2.wav", GL_FALSE);
				meteor->setNeedsToBeDeleted(true);

				auto entity = dynamic_cast<Entity*>(collider.get());
				if (entity != nullptr && !entity->getNeedsToBeDeleted())
				{
					if (entity->getAddon("shield") != nullptr)
						entity->removeAddon("shield");
					else
						entity->setNeedsToBeDeleted(true);
				}
			};
			meteors.push_back(std::move(meteor));
		}

		initPlayerUI();

		accumulator = 0.0f;
		t = 0.0f;
	}

	void Application::initGameUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_INIT_WINDOW_WIDTH)), (float)(glutGet(GLUT_INIT_WINDOW_HEIGHT)));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), nullptr, glm::vec2(30.0f, 30.0f))));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		ui.push_back(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		auto Options = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), getUIElement("Main Menu"), glm::vec2(0.0f, 0.0f));
		auto Controls = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), Options, glm::vec2(0.0f, 0.0f));

		//Main Menu
		auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			explosions.clear();
			initScene();
			setState(GameState::STARTED);
			currentMenu = nullptr;
			//updatePlayerUI();
			getUIElement("Main Menu")->hideMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);

			currentMenu = Options;
			getUIElement("Main Menu")->hideMain();
			Options->showMain();
		};
		getUIElement("Main Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = nullptr;
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		getUIElement("Main Menu")->addText(std::move(options));
		getUIElement("Main Menu")->showMain(true);

		//Pause Menu
		options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

			currentMenu = getUIElement("Main Menu");
			setState(GameState::NOTSTARTEDYET);
			getUIElement("Pause Menu")->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		getUIElement("Pause Menu")->addText(std::move(options));
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = nullptr;
			#if _DEBUG
				std::cout << "exiting" << std::endl;
				getchar();
			#endif
			exit(0);
		};
		getUIElement("Pause Menu")->addText(std::move(options));
		
		//Controls
		auto keybindings = inputManager->getKeyBindings();
		size_t i = 0;
		for (auto it = keybindings->begin(); it != keybindings->end(); it++)
		{
			options = std::make_shared<Text>(it->first + ": ", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
			options->setIsStatic(true);
			Controls->addText(std::move(options));
			options = std::make_shared<Text>(virtualKeyCodeToString(it->second), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
			options->onMouseClickFunc = [this, options, it]()
			{
				soundEngine->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

				inputManager->resetCurrentEditedKeyBinding();
				inputManager->setCurrentEditedKeyBinding(std::pair<std::vector<std::pair<std::string, int>>::iterator, std::shared_ptr<Text>>(it, options));
				options->setIsStatic(true);
				options->changeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			};
			Controls->addText(std::move(options));
			i++;
		}
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = Options;
			Controls->hideMain();
			Options->showMain();
		};
		Controls->addText(std::move(options));

		//Options
		options = std::make_shared<Text>("Controls", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentMenu = Controls;
			Options->hideMain();
			Controls->showMain();
		};
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Sounds", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		Options->addText(std::move(options));
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			soundEngine->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
			currentMenu = getUIElement("Main Menu");
			Options->hideMain();
			getUIElement("Main Menu")->showMain();
		};
		Options->addText(std::move(options));
		Options->addUIElement(std::move(Controls));
		getUIElement("Main Menu")->addUIElement(std::move(Options));

		//Game Over
		options = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		options->setIsStatic(true);
		getUIElement("Game Over")->addText(std::move(options));

		getUIElement("Game Over")->hideMain();
		getUIElement("Pause Menu")->hideMain();
	}

	void Application::startNewLevel()
	{
		getPlayerUIElement("Level completed")->hideMain();
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto listOfAllElements = std::vector<std::shared_ptr<UIElement>>();
		auto listOfAllTexts = std::vector<std::shared_ptr<Text>>();

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

				//for (auto it = enemies.begin(); it != enemies.end();)
				//{
				//	if ((*it)->update(dt, t))
				//		removeEnemyFromList(&it);
				//	else
				//		it++;
				//}

				for (auto it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt))
						removeEnemyFromList(&it);
					else
						it++;
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

				//Collision detection
				if (player->getPosition(0) + player->getSize(0) >= windowWidth)
					player->setPosition(0, windowWidth - player->getSize(0));
				else if (player->getPosition(0) <= 0.0f)
					player->setPosition(0, 0.0f);

				if (player->getPosition(1) + player->getSize(1) >= windowHeigth)
					player->setPosition(1, windowHeigth - player->getSize(1));
				else if (player->getPosition(1) <= 0.0f)
					player->setPosition(1, 0.0f);

				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					auto playerBulletList = player->getBulletsList();
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

				t += dt;
			}
			accumulator -= dt;
		}

		if (getState() == GameState::STARTED)
		{
			//updatePlayerUI();

			//Render background
			renderer->draw(background);
			//Render player
			renderer->draw(player);
			auto addons = player->getAddons();
			for (auto it = addons->begin(); it != addons->end(); it++)
			{
				renderer->draw(it->second);
			}
			//Render enemies
			for (auto it = enemies.begin(); it != enemies.end(); it++)
			{
				auto addons = (*it)->getAddons();
				renderer->draw(*it);
				for (auto it2 = addons->begin(); it2 != addons->end(); it2++)
				{
					renderer->draw(it2->second);
				}
			}
			//Render pickups
			renderer->draw(pickups);
			//Render explosions
			renderer->draw(explosions);
			//Render bullets
			for (auto it = enemies.begin(); it != enemies.end(); it++)
			{
				auto enemyBulletList = (*it)->getBulletsList();
				renderer->draw(*enemyBulletList);
			}

			renderer->draw(*player->getBulletsList());
			//Render meteors
			renderer->draw(meteors);
			//Render & Update player UI
			for (auto uiElement : playerUI)
			{
				uiElement.second->update(dt);
				listOfAllElements.push_back(uiElement.second);
				uiElement.second->GetAllChildrenElements(&listOfAllElements);
				uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
			}

			renderer->draw(listOfAllElements);
			renderer->draw(listOfAllTexts);

			listOfAllElements.clear();
			listOfAllTexts.clear();
		}

		//Render & Update UI
		for (auto uiElement : ui)
		{
			uiElement.second->update(dt);
			listOfAllElements.push_back(uiElement.second);
			uiElement.second->GetAllChildrenElements(&listOfAllElements);
			uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
		}

		renderer->draw(listOfAllElements);
		renderer->draw(listOfAllTexts);

		glutSwapBuffers();
	}

	void Application::resize(int width, int height)
	{
		//const float ar = (float) width / (float) height;

		for (auto uiElement : ui)
			uiElement.second->fixPosition();

		for (auto uiElement : playerUI)
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
						currentMenu = getUIElement("Main Menu");
						getUIElement("Game Over")->hideMain();
						getUIElement("Main Menu")->showMain();
						setState(GameState::NOTSTARTEDYET);
					}
					else if (getState() == GameState::PAUSED)
					{
						currentMenu = nullptr;
						getUIElement("Pause Menu")->hideMain();
						setState(GameState::STARTED);
					}
					else if (getState() == GameState::STARTED)
					{
						currentMenu = getUIElement("Pause Menu");
						getUIElement("Pause Menu")->showMain(false);
						setState(GameState::PAUSED);
					}
					else if (getState() == GameState::NOTSTARTEDYET)
					{
						if (!inputManager->resetCurrentEditedKeyBinding() && currentMenu != nullptr && currentMenu->getParent() != nullptr)
						{
							soundEngine->play2D("Sounds/buttonselect/6.wav", GL_FALSE);
							currentMenu->hideMain();
							currentMenu = currentMenu->getParent();
							currentMenu->showMain();
						}
					}
					break;
				}
			}

			auto keyBindings = inputManager->getKeyBindings();
			auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
			if (c >= 32 && c < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [key](std::pair<std::string, int> element){return element.second == key; }) && currentKeyBinding->second != nullptr)
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
		if (c != 0 && !std::any_of(keyBindings->begin(), keyBindings->end(), [c](std::pair<std::string, int> element){return element.second == c; }) && currentKeyBinding->second != nullptr)
		{
			soundEngine->play2D("Sounds/buttonselect/2.wav", GL_FALSE);
			currentKeyBinding->first->second = c;
			currentKeyBinding->second->setText(charText);
			inputManager->resetCurrentEditedKeyBinding();
		}
	}

	void Application::motionFunc(int x, int y)
	{
		inputManager->setLastMousePosition(glm::vec2(x, y));

		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		for (auto uiElement : ui)
		{
			uiElement.second->checkIfMouseHoverThis(lastMousePosition);
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

			for (auto uiElement : ui)
			{
				uiElement.second->checkForMouseClickOnThis(inputManager->getLeftMouseState(), inputManager->getLastLeftMouseState(), lastMousePosition);
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

	std::shared_ptr<UIElement> Application::getUIElement(std::string index)
	{
		for (auto uiElement : ui)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	std::shared_ptr<UIElement> Application::getPlayerUIElement(std::string index)
	{
		for (auto uiElement : playerUI)
		{
			if (uiElement.first == index)
				return uiElement.second;
		}

		return nullptr;
	}

	void Application::erasePlayerUIElement(std::string index)
	{
		for (auto it = playerUI.begin(); it != playerUI.end(); it++)
		{
			if (it->first == index)
			{
				playerUI.erase(it);
				return;
			}
		}
	}

	float Application::randomFloat(float min, float max)
	{
		// this  function assumes max > min, you may want 
		// more robust error checking for a non-debug build
		assert(max > min);
		float random = ((float)rand()) / (float)RAND_MAX;

		// generate (in your case) a float between 0 and (4.5-.78)
		// then add .78, giving you a float between .78 and 4.5
		float range = max - min;
		return (random*range) + min;
	}
}