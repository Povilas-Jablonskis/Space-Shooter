#include "Application.h"

namespace Engine
{
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
		else {
		std::cout << "config.json not found" << std::endl;
		}*/

		renderer->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
		renderer->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

		fontManager->loadFont("kenvector_future.ttf", "kenvector_future");
		fontManager->loadFont("kenvector_future_thin.ttf", "kenvector_future_thin");

		textureManager->loadTexture("PNG/UI/playerLife1_blue.png", "playerLife1_blue", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/playerShip1_blue.png", "playerShip1_blue", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Enemies/enemyBlack1.png", "enemyBlack1", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("Backgrounds/blue.png", "blueBackground", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/laserBlue01.png", "laserBlue01", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/laserGreen11.png", "laserGreen11", 0, 0, glm::vec2(1, 1));
		textureManager->loadTexture("PNG/Lasers/blueExplosionSpriteSheet.png", "blueExplosionSpriteSheet", 1, 2, glm::vec2(2, 1));

		initGameUI();
		currentMenu = ui["Main Menu"];

		background = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), nullptr, glm::vec2(0.0f, 0.0f));
		background->applyTexture(std::make_shared<Texture>(*textureManager->getTexture("blueBackground")));
		player = std::make_shared<Player>(32, 32, glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f), glm::vec2(80.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
		player->applyTexture(std::make_shared<Texture>(*textureManager->getTexture("playerShip1_blue")));
		player->addAnimation("shoot", std::make_shared<Texture>(*textureManager->getTexture("laserBlue01")));
		player->addAnimation("explosion", std::make_shared<Texture>(*textureManager->getTexture("blueExplosionSpriteSheet")));

		initPlayerUI();
	}

	Application::~Application()
	{

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

	void Application::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<BaseGameObject>>* colliderlist) // AABB - AABB collision
	{
		auto collision = collisionManager->checkCollision(object, colliderlist);
		if (collision != colliderlist->end())
			(*collision)->onCollision(object.get());
	}

	void Application::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<Bullet>>* bulletList, std::shared_ptr<BaseGameObject> parent) // AABB - AABB collision
	{
		auto collision = collisionManager->checkCollision(object, bulletList);
		if (collision != bulletList->end())
		{
			auto explosion = std::make_shared<Explosion>(32, 32, object->getPosition());
			explosion->applyTexture(object->getAnimation("explosion"));
			explosions.push_back(explosion);
			(*collision)->onCollision(object.get(), parent.get());
		}
	}

	void Application::initPlayerUI()
	{
		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
		option->setIsStatic(true);
		playerUI["Score"]->addText(option);

		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option2 = std::make_shared<UIElement>(33, 26, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
		option2->applyTexture(std::make_shared<Texture>(*textureManager->getTexture("playerLife1_blue")));
		playerUI["Health"]->addUIElement(option2);
		option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
		option->setIsStatic(true);
		playerUI["Health"]->addText(option);

		playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

		auto option3 = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		option3->setIsStatic(true);
		playerUI["Level completed"]->addText(option3);

		playerUI["Score"]->fixPosition();
		playerUI["Health"]->fixPosition();
		playerUI["Level completed"]->fixPosition();
	}

	void Application::updatePlayerUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		for (std::unordered_map<std::string, std::shared_ptr<UIElement>>::iterator it = playerUI.begin(); it != playerUI.end();)
		{
			if (it->first == "Score" && player->getLastScore() != player->getScore())
			{
				it = playerUI.erase(it);
				playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

				//Score
				auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
				option->setIsStatic(true);
				playerUI["Score"]->addText(option);
				playerUI["Score"]->fixPosition();
				break;
			}
			else if (it->first == "Health" && player->getLastHealth() != player->getHealth())
			{
				it = playerUI.erase(it);
				playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

				//Health
				auto option2 = std::make_shared<UIElement>(33, 26, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
				option2->applyTexture(std::make_shared<Texture>(*textureManager->getTexture("playerLife1_blue")));
				playerUI["Health"]->addUIElement(option2);
				auto option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
				option->setIsStatic(true);
				playerUI["Health"]->addText(option);
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
		for (size_t i = 0; i < ((glutGet(GLUT_WINDOW_WIDTH) - 32) / space); i++)
		{
			auto enemy = std::make_shared<TestEnemy>(32, 32, glm::vec2(i * space, 416.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
			enemy->addAnimation("shoot", std::make_shared<Texture>(*textureManager->getTexture("laserGreen11")));
			enemy->addAnimation("explosion", std::make_shared<Texture>(*textureManager->getTexture("blueExplosionSpriteSheet")));
			enemy->applyTexture(std::make_shared<Texture>(*textureManager->getTexture("enemyBlack1")));

			enemies.push_back(enemy);
		}

		accumulator = 0.0f;
		t = 0.0f;
	}

	void Application::initGameUI()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
		auto Options = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), ui["Main Menu"], glm::vec2(0.0f, 0.0f));
		auto C = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), Options, glm::vec2(0.0f, 0.0f));

		//Main Menu
		auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
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
		ui["Main Menu"]->addText(options);
		options = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			currentMenu = Options;
			ui["Main Menu"]->hideMain();
			Options->showMain();
		};
		ui["Main Menu"]->addText(options);
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = nullptr;
			std::cout << "exiting" << std::endl;
			getchar();
			exit(0);
		};
		ui["Main Menu"]->addText(options);
		ui["Main Menu"]->showMain();

		//Pause Menu
		options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = ui["Main Menu"];
			setState(GameState::NOTSTARTEDYET);
			ui["Pause Menu"]->hideMain();
			ui["Main Menu"]->showMain();
		};
		ui["Pause Menu"]->addText(options);
		options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
		options->onMouseReleaseFunc = [this]()
		{
			currentMenu = nullptr;
			std::cout << "exiting" << std::endl;
			getchar();
			exit(0);
		};
		ui["Pause Menu"]->addText(options);

		//Options
		options = std::make_shared<Text>("A", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		options->onMouseReleaseFunc = [this, Options, C]()
		{
			currentMenu = C;
			Options->hideMain();
			C->showMain();
		};
		Options->addText(options);
		options = std::make_shared<Text>("B", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		Options->addText(options);
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
		options->onMouseReleaseFunc = [this, Options]()
		{
			currentMenu = ui["Main Menu"];
			Options->hideMain();
			ui["Main Menu"]->showMain();
		};
		Options->addText(options);
		ui["Main Menu"]->addUIElement(Options);

		//C
		options = std::make_shared<Text>("C", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
		C->addText(options);
		options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
		options->onMouseReleaseFunc = [this, Options, C]()
		{
			currentMenu = Options;
			C->hideMain();
			Options->showMain();
		};
		C->addText(options);
		Options->addUIElement(C);

		//Game Over
		options = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
		options->setIsStatic(true);
		ui["Game Over"]->addText(options);
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
						it = enemies.erase(it);
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
				checkCollision(player, (*it)->getBulletsList(), *it);
				checkCollision(*it, playerBulletList, player);
			}

			checkCollision(player, &std::vector<std::shared_ptr<BaseGameObject>>(enemies.begin(), enemies.end()));

			if (player->getHealth() < 1)
			{
				setState(GameState::ENDED);
				currentMenu = ui["Game Over"];
				ui["Game Over"]->showMain();
			}

			if (enemies.size() == 0)
			{
				timer([this]{ startNewLevel(); }, 2000);
				playerUI["Level completed"]->showMain();
			}

			updatePlayerUI();

			//Render background
			renderer->draw(background);
			//Render player
			renderer->draw(player);
			//Render enemies
			renderer->draw(std::vector<std::shared_ptr<RenderObject>>(enemies.begin(), enemies.end()));

			//Render explosions
			renderer->draw(std::vector<std::shared_ptr<RenderObject>>(explosions.begin(), explosions.end()));

			//Render bullets
			for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end(); it++)
			{
				auto enemyBulletList = (*it)->getBulletsList();
				renderer->draw(std::vector<std::shared_ptr<RenderObject>>(enemyBulletList->begin(), enemyBulletList->end()));
			}

			renderer->draw(std::vector<std::shared_ptr<RenderObject>>(playerBulletList->begin(), playerBulletList->end()));

			//Render & Update player UI
			for (auto uiElement : playerUI)
			{
				uiElement.second->update(dt);
				uiElement.second->GetAllChildrenElements(&listOfAllElements);
				uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
			}

			renderer->draw(std::vector<std::shared_ptr<RenderObject>>(listOfAllElements.begin(), listOfAllElements.end()));
			renderer->draw(listOfAllTexts);

			listOfAllElements.clear();
			listOfAllTexts.clear();
		}

		//Render & Update UI
		for (auto uiElement : ui)
		{
			uiElement.second->update(dt);
			uiElement.second->GetAllChildrenElements(&listOfAllElements);
			uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
		}

		renderer->draw(std::vector<std::shared_ptr<RenderObject>>(listOfAllElements.begin(), listOfAllElements.end()));
		renderer->draw(listOfAllTexts);

		glutSwapBuffers();
	}

	void Application::keyboardInputUp(unsigned char c, int x, int y)
	{
		if (inputManager->getKey(c))
			inputManager->setKey(c, false);
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
					ui["Pause Menu"]->showMain();
					setState(GameState::PAUSED);
				}
				else if (getState() == GameState::NOTSTARTEDYET)
				{
					if (currentMenu != nullptr && currentMenu->getParent() != nullptr)
					{
						currentMenu->hideMain();
						currentMenu = currentMenu->getParent();
						currentMenu->showMain();
					}
				}
				break;
			}
			}
			inputManager->setKey(c, true);
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
				uiElement.second->checkForMouseClickOnThis(inputManager, lastMousePosition);
			}
		}
		/*if (button == GLUT_RIGHT_BUTTON)
		state == GLUT_UP ? inputManager->SetRightMouseState(true), inputManager->SetLastRightMouseState(false) : inputManager->SetRightMouseState(false), inputManager->SetLastRightMouseState(true);*/
	}
}