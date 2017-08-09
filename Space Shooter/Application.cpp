#include "Application.h"

namespace Engine
{
	std::shared_ptr<Application> Application::instance_;

	std::shared_ptr<Application> Application::instance()
	{
		// Lazy initialize.
		if (instance_ == nullptr) instance_ = std::make_shared<Application>();
		return instance_;
	}

	Application::Application() 
		: collisionManager(std::make_shared<CollisionManager>()), renderer(std::make_shared<Renderer>()), fontManager(std::make_shared<FontManager>()), inputManager(std::make_shared<InputManager>()), textureManager(std::make_shared<TextureManager>()), gameState(GameState::NOTSTARTEDYET)
	{
		currentTime = (float)(glutGet(GLUT_ELAPSED_TIME));
		accumulator = 0.0f;
		dt = 1.0f / 60.0f;
		t = 0.0f;

		/*char writeBuffer[65536];
		FILE* pFile;

		fopen_s(&pFile,"config.json", "rb");
		if (pFile != NULL)
		{
			rapidjson::FileReadStream is(pFile, writeBuffer, sizeof(writeBuffer));
			rapidjson::Document d;
			d.ParseStream(is);
			fclose(pFile);

			const rapidjson::Value& second = d["second"];
			const rapidjson::Value& first = d["first"];

			for (rapidjson::Value::ConstMemberIterator itr = second[0].MemberBegin(); itr != second[0].MemberEnd(); ++itr) {
				std::cout << itr->name.GetString() << ":" << itr->value.GetType() << std::endl;
			}

			std::cout << std::endl << std::endl;

			for (rapidjson::Value::ConstMemberIterator itr = first[0].MemberBegin(); itr != first[0].MemberEnd(); ++itr) {
				std::cout << itr->name.GetString() << ":" << itr->value.GetType() << std::endl;
			}
		}
		else 
			std::cout << "config.json not found" << std::endl;
		*/

		inputManager->setKeyBinding("Attack", 32);
		inputManager->setKeyBinding("Move_Left", 97);
		inputManager->setKeyBinding("Move_Right", 100);
		inputManager->setKeyBinding("Move_Back", 115);

		renderer->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		renderer->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		fontManager->loadFont("kenvector_future.ttf", "kenvector_future");
		fontManager->loadFont("kenvector_future_thin.ttf", "kenvector_future_thin");

		textureManager->loadTexture("PNG/UI/playerLife1_blue.png", "playerLife1_blue", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/playerShip1_blue.png", "playerShip1_blue", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Enemies/enemyBlack1.png", "enemyBlack1", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("Backgrounds/blue.png", "blueBackground", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/laserBlue01.png", "laserBlue01", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/blueExplosionSpriteSheet.png", "blueExplosionSpriteSheet", 1, 2, glm::vec2(2, 1));
		textureManager->loadTexture("PNG/Lasers/laserGreen11.png", "laserGreen11", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/greenExplosionSpriteSheet.png", "greenExplosionSpriteSheet", 1, 2, glm::vec2(2, 1));
		textureManager->loadTexture("PNG/Effects/shieldSpriteSheet.png", "shieldSpriteSheet", 1, 3, glm::vec2(3, 1));

		textureManager->getTexture("shieldSpriteSheet")->setLoopStatus(true);
		textureManager->getTexture("shieldSpriteSheet")->setDelay(0.15f);

		background = std::make_shared<UIElement>((float)glutGet(GLUT_INIT_WINDOW_WIDTH), (float)glutGet(GLUT_INIT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), nullptr, glm::vec2(0.0f, 0.0f));
		background->applyTexture(textureManager->getTexture("blueBackground"));
		player = std::make_shared<Player>(32.0f, 32.0f, glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f), glm::vec2(80.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		player->onDeath = [this]()
		{
			setState(GameState::ENDED);
			currentMenu = ui["Game Over"];
			ui["Game Over"]->showMain(false);
		};
		player->applyTexture(textureManager->getTexture("playerShip1_blue"));
		player->addAnimation("shoot", textureManager->getTexture("laserBlue01"));
		player->addAnimation("explosion", textureManager->getTexture("blueExplosionSpriteSheet"));

		auto shield = std::make_shared<Addon>(48.0f, 48.0f, glm::vec2(-8.0f, -6.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
		shield->applyTexture(textureManager->getTexture("shieldSpriteSheet"));
		player->addAddon(std::pair<std::string, std::shared_ptr<Addon>>("shield", std::move(shield)));

		initGameUI();
		currentMenu = ui["Main Menu"];

		initPlayerUI();
	}

	Application::~Application()
	{
		enemies.clear();
		explosions.clear();
		ui.clear();
		playerUI.clear();
	}

	template <class T>
	void Application::timer(const T& callback, unsigned int timeInMs)
	{
		concurrency::task_completion_event<void> tce;
		auto call = new concurrency::call<int>(
			[callback, tce](int)
		{
			callback();
			tce.set();
		});

		auto timer = new concurrency::timer<int>(timeInMs, 0, call, false);
		concurrency::task<void> event_set(tce);
		event_set.then([timer, call]()
		{
			delete call;
			delete timer;
		});

		timer->start();
	}

	void Application::removeEnemyFromList(std::vector<std::shared_ptr<TestEnemy>>::iterator* it)
	{
		if (*it != enemies.end())
			*it = enemies.erase(*it);

		if (enemies.size() == 0)
		{
			timer([this]{ startNewLevel(); }, 2000);
			playerUI["Level completed"]->showMain(false);
		}
	}

	void Application::removeExplosionFromList(std::vector<std::shared_ptr<Explosion>>::iterator* it)
	{
		if (*it != explosions.end())
			*it = explosions.erase(*it);
	}

	void Application::initPlayerUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_INIT_WINDOW_WIDTH)), (float)(glutGet(GLUT_INIT_WINDOW_HEIGHT)));

		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		playerUI["Score"]->addText(std::move(option));

		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option2 = std::make_shared<UIElement>(33.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyTexture(textureManager->getTexture("playerLife1_blue"));
		playerUI["Health"]->addUIElement(std::move(option2));
		option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
		option->setIsStatic(true);
		playerUI["Health"]->addText(std::move(option));

		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option3 = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		option3->setIsStatic(true);
		playerUI["Level completed"]->addText(std::move(option3));

		playerUI["Score"]->fixPosition();
		playerUI["Health"]->fixPosition();
		playerUI["Level completed"]->fixPosition();
	}

	void Application::updatePlayerUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		for (std::map<std::string, std::shared_ptr<UIElement>>::iterator it = playerUI.begin(); it != playerUI.end();)
		{
			if (it->first == "Score" && player->getLastScore() != player->getScore())
			{
				it = playerUI.erase(it);
				playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

				//Score
				auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
				option->setIsStatic(true);
				playerUI["Score"]->addText(std::move(option));
				playerUI["Score"]->fixPosition();
				break;
			}
			else if (it->first == "Health" && player->getLastHealth() != player->getHealth())
			{
				it = playerUI.erase(it);
				playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

				//Health
				auto option2 = std::make_shared<UIElement>(33.0f, 26.0f, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
				option2->applyTexture(textureManager->getTexture("playerLife1_blue"));
				playerUI["Health"]->addUIElement(std::move(option2));
				auto option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
				option->setIsStatic(true);
				playerUI["Health"]->addText(std::move(option));
				playerUI["Health"]->fixPosition();
				break;
			}
			else
				++it;
		}
	}

	void Application::initScene()
	{
		enemies.clear();
		float space = 60.f;
		for (size_t i = 0; i < ((glutGet(GLUT_INIT_WINDOW_WIDTH) - 32) / space); i++)
		{
			auto enemy = std::make_shared<TestEnemy>(32.0f, 32.0f, glm::vec2(i * space, 416.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
			enemy->addAnimation("shoot", textureManager->getTexture("laserGreen11"));
			enemy->addAnimation("explosion", textureManager->getTexture("greenExplosionSpriteSheet"));
			enemy->applyTexture(textureManager->getTexture("enemyBlack1"));
			enemies.push_back(std::move(enemy));
		}

		accumulator = 0.0f;
		t = 0.0f;
	}

	void Application::initGameUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_INIT_WINDOW_WIDTH)), (float)(glutGet(GLUT_INIT_WINDOW_HEIGHT)));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), nullptr, glm::vec2(30.0f, 30.0f))));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(temPos.x, temPos.y, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		auto Options = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), ui["Main Menu"], glm::vec2(0.0f, 0.0f));
		auto Controls = std::make_shared<UIElement>(temPos.x / 2.0f, temPos.y / 2.0f, glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), Options, glm::vec2(0.0f, 0.0f));

		//Main Menu
		auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			initScene();

			if (player != nullptr)
				player->restart();

			explosions.clear();
			setState(GameState::STARTED);
			currentMenu = nullptr;
			updatePlayerUI();
			ui["Main Menu"]->hideMain();
		};
		ui["Main Menu"]->addText(std::move(options));
		options = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			currentMenu = Options;
			ui["Main Menu"]->hideMain();
			Options->showMain();
		};
		ui["Main Menu"]->addText(std::move(options));
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
		ui["Main Menu"]->addText(std::move(options));
		ui["Main Menu"]->showMain(true);

		//Pause Menu
		options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = ui["Main Menu"];
			setState(GameState::NOTSTARTEDYET);
			ui["Pause Menu"]->hideMain();
			ui["Main Menu"]->showMain();
		};
		ui["Pause Menu"]->addText(std::move(options));
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
		ui["Pause Menu"]->addText(std::move(options));
		
		//Controls
		auto keybindings = inputManager->getKeyBindings();
		size_t i = 0;
		for (std::map<std::string, int>::iterator it = keybindings->begin(); it != keybindings->end(); it++)
		{
			auto characterMoveName = it->first;
			std::replace(characterMoveName.begin(), characterMoveName.end(), '_', ' ');
			options = std::make_shared<Text>(characterMoveName + ": ", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
			options->setIsStatic(true);
			Controls->addText(std::move(options));
			switch (it->second)
			{
				case 32:
				{
					options = std::make_shared<Text>("Space", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
				case 128:
				{
					options = std::make_shared<Text>("Arrow Up", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
				case 129:
				{
					options = std::make_shared<Text>("Arrow Down", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
				case 130:
				{
					options = std::make_shared<Text>("Arrow Left", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
				case 131:
				{
					options = std::make_shared<Text>("Arrow Right", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
				default:
				{
					options = std::make_shared<Text>(it->second, 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f - (10 * i)));
					break;
				}
			}
			options->onMouseClickFunc = [this, options, it]()
			{
				inputManager->resetCurrentEditedKeyBinding();
				inputManager->setCurrentEditedKeyBinding(std::pair<std::string, std::shared_ptr<Text>>(it->first, options));
				options->setIsStatic(true);
				options->changeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			};
			Controls->addText(std::move(options));
			i++;
		}
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(20.0f, 60.0f - (10 * i)));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
			currentMenu = Options;
			Controls->hideMain();
			Options->showMain();
		};
		Controls->addText(std::move(options));

		//Options
		options = std::make_shared<Text>("Controls", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		options->onMouseReleaseFunc = [this, Options, Controls]()
		{
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
			currentMenu = ui["Main Menu"];
			Options->hideMain();
			ui["Main Menu"]->showMain();
		};
		Options->addText(std::move(options));
		Options->addUIElement(std::move(Controls));
		ui["Main Menu"]->addUIElement(std::move(Options));

		//Game Over
		options = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), fontManager->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		options->setIsStatic(true);
		ui["Game Over"]->addText(std::move(options));

		ui["Game Over"]->hideMain();
		ui["Pause Menu"]->hideMain();
	}

	void Application::startNewLevel()
	{
		playerUI["Level completed"]->hideMain();
	}

	void Application::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		auto listOfAllElements = std::vector<std::shared_ptr<UIElement>>();
		auto listOfAllTexts = std::vector<std::shared_ptr<Text>>();

		float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
		float frameTime = (newTime - currentTime) / 1000.0f;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			if (getState() == GameState::STARTED)
			{
				player->update(dt, inputManager);

				for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end();)
				{
					if ((*it)->update(dt, t))
						removeEnemyFromList(&it);
					else
						it++;
				}

				for (std::vector<std::shared_ptr<Explosion>>::iterator it = explosions.begin(); it != explosions.end();)
				{
					if ((*it)->update(dt))
						it = explosions.erase(it);
					else
						++it;
				}

				t += dt;
			}
			accumulator -= dt;
		}

		if (getState() == GameState::STARTED)
		{
			auto playerBulletList = player->getBulletsList();

			for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end(); it++)
			{
				collisionManager->checkCollision(player, (*it)->getBulletsList(), *it);
				collisionManager->checkCollision(*it, playerBulletList, player);
			}

			collisionManager->checkCollision(player, &enemies);

			updatePlayerUI();

			//Render background
			renderer->draw(background);
			//Render player
			renderer->draw(player);
			auto addons = player->getAddons();
			for (std::map<std::string, std::shared_ptr<Addon>>::iterator it = addons->begin(); it != addons->end(); it++)
			{
				renderer->draw(it->second);
			}
			//Render enemies
			renderer->draw(enemies);
			//Render explosions
			renderer->draw(explosions);
			//Render bullets
			for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end(); it++)
			{
				auto enemyBulletList = (*it)->getBulletsList();
				renderer->draw(*enemyBulletList);
			}

			renderer->draw(*playerBulletList);
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

		glutReshapeWindow(width, height);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	}

	void Application::keyboardInputUp(unsigned char c, int x, int y)
	{
		if (inputManager->getKey(c))
			inputManager->setKey(c, false);
	}

	void Application::keyboardInput(unsigned char c, int x, int y)
	{
		if (!inputManager->getKey(c))
		{
			switch (c)
			{
				case 27:
				{
					if (getState() == GameState::ENDED)
					{
						currentMenu = ui["Main Menu"];
						ui["Game Over"]->hideMain();
						ui["Main Menu"]->showMain();
						setState(GameState::NOTSTARTEDYET);
					}
					else if (getState() == GameState::PAUSED)
					{
						currentMenu = nullptr;
						ui["Pause Menu"]->hideMain();
						setState(GameState::STARTED);
					}
					else if (getState() == GameState::STARTED)
					{
						currentMenu = ui["Pause Menu"];
						ui["Pause Menu"]->showMain(false);
						setState(GameState::PAUSED);
					}
					else if (getState() == GameState::NOTSTARTEDYET)
					{
						if (!inputManager->resetCurrentEditedKeyBinding() && currentMenu != nullptr && currentMenu->getParent() != nullptr)
						{
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
			if (c >= 32 && c < 127 && !std::any_of(keyBindings->begin(), keyBindings->end(), [c](std::pair<std::string, int> element){return element.second == c; }) && keyBindings->find(currentKeyBinding->first) != keyBindings->end())
			{
				auto currentKey = keyBindings->find(currentKeyBinding->first);
				currentKey->second = c;
				if (c == 32)
					currentKeyBinding->second->setText("Space");
				else
					currentKeyBinding->second->setText(c);
				inputManager->resetCurrentEditedKeyBinding();
			}
			inputManager->setKey(c, true);
		}
	}

	void Application::specialKeyInputUp(int key, int x, int y)
	{
		switch (key)
		{
			case GLUT_KEY_UP:
			{
				if (inputManager->getKey(128))
					inputManager->setKey(128, false);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				if (inputManager->getKey(129))
					inputManager->setKey(129, false);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				if (inputManager->getKey(130))
					inputManager->setKey(130, false);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				if (inputManager->getKey(131))
					inputManager->setKey(131, false);
				break;
			}
		}
	}

	void Application::specialKeyInput(int key, int x, int y)
	{
		auto keyBindings = inputManager->getKeyBindings();
		auto currentKeyBinding = inputManager->getCurrentEditedKeyBinding();
		int c = 0;
		std::string charText = "";
		switch (key)
		{
			case GLUT_KEY_UP:
			{
				charText = "Arrow Up";
				c = 128;
				if (!inputManager->getKey(128))
					inputManager->setKey(128, true);
				break;
			}
			case GLUT_KEY_DOWN:
			{
				charText = "Arrow Down";
				c = 129;
				if (!inputManager->getKey(129))
					inputManager->setKey(129, true);
				break;
			}
			case GLUT_KEY_LEFT:
			{
				charText = "Arrow Left";
				c = 130;
				if (!inputManager->getKey(130))
					inputManager->setKey(130, true);
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				charText = "Arrow Right";
				c = 131;
				if (!inputManager->getKey(131))
					inputManager->setKey(131, true);
				break;
			}
		}

		if (c != 0 && !std::any_of(keyBindings->begin(), keyBindings->end(), [c](std::pair<std::string, int> element){return element.second == c; }) && keyBindings->find(currentKeyBinding->first) != keyBindings->end())
		{
			auto currentKey = keyBindings->find(currentKeyBinding->first);
			currentKey->second = c;
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
			inputManager->setLeftMouseState(state == GLUT_DOWN ? true : false);
			inputManager->setLastLeftMouseState(state == GLUT_DOWN ? false : true);

			for (auto uiElement : ui)
			{
				uiElement.second->checkForMouseClickOnThis(inputManager->getLeftMouseState(), inputManager->getLastLeftMouseState(), lastMousePosition);
			}
		}
		/*if (button == GLUT_RIGHT_BUTTON)
		state == GLUT_UP ? inputManager->SetRightMouseState(true), inputManager->SetLastRightMouseState(false) : inputManager->SetRightMouseState(false), inputManager->SetLastRightMouseState(true);*/
	}
}