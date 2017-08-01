#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <map>
#include <string>

#include "BaseGameObject.h"
#include "Shader.h"
#include "Player.h"
#include "TestEnemy.h"
#include "UIElement.h"
#include "UIElementBase.h"
#include "Text.h"
#include "GameState.h"
#include "Application.h"
#include "Explosion.h"
#include "Renderer.h"
#include "CollisionManager.h"

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace Engine;

std::shared_ptr<Application> application;
std::shared_ptr<Player> player;
std::shared_ptr<UIElement> background;

std::shared_ptr<UIElement> currentMenu;

float t;
float dt;
float currentTime;
float accumulator;

std::vector<std::shared_ptr<TestEnemy>> enemies;
std::vector<std::shared_ptr<Explosion>> explosions;
std::unordered_map<std::string, std::shared_ptr<UIElement>> ui;
std::unordered_map<std::string, std::shared_ptr<UIElement>> playerUI;

void initPlayerUI()
{
	playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

	auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), application->getFont("kenvector_future"), glm::vec2(90.0f, 93.0f));
	option->setIsStatic(true);
	playerUI["Score"]->addText(option);

	playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

	auto option2 = std::make_shared<UIElement>(33, 26, glm::vec2(0.0f, 0.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f), nullptr, glm::vec2(6.0f, 91.0f));
	option2->applyTexture(std::make_shared<Texture>(*application->getTexture("playerLife1_blue")));
	playerUI["Health"]->addUIElement(option2);
	option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), application->getFont("kenvector_future"), glm::vec2(12.0f, 92.0f));
	option->setIsStatic(true);
	playerUI["Health"]->addText(option);

	playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Level completed", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));

	auto option3 = std::make_shared<Text>("Level completed!", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
	option3->setIsStatic(true);
	playerUI["Level completed"]->addText(option3);

	playerUI["Score"]->fixPosition();
	playerUI["Health"]->fixPosition();
	playerUI["Level completed"]->fixPosition();
}

void initScene()
{
	enemies.clear();
	for (size_t i = 0; i < 16; i++)
	{
		auto enemy = std::make_shared<TestEnemy>(32, 32, glm::vec2(i * 40.0f, 416.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
		enemy->applyTexture(std::make_shared<Texture>(*application->getTexture("enemyBlack1")));
		enemies.push_back(enemy);
	}

	accumulator = 0.0f;
	t = 0.0f;
}

void initGameUI()
{
	glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
	auto Options = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), ui["Main Menu"], glm::vec2(0.0f, 0.0f));
	auto C = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), Options, glm::vec2(0.0f, 0.0f));

	//Main Menu
	auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(48.0f, 60.0f));
	options->onMouseReleaseFunc = []()
	{
		initScene();

		if (player != nullptr)
			player->restart();

		explosions.clear();
		application->setState(GameState::STARTED);
		currentMenu = nullptr;
		ui["Main Menu"]->hideMain();
	};
	ui["Main Menu"]->addText(options);
	options = std::make_shared<Text>("Options", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
	options->onMouseReleaseFunc = [Options]()
	{
		currentMenu = Options;
		ui["Main Menu"]->hideMain();
		Options->showMain();
	};
	ui["Main Menu"]->addText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(48.0f, 50.0f));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = nullptr;
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	ui["Main Menu"]->addText(options);
	ui["Main Menu"]->showMain();

	//Pause Menu
	options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(45.0f, 60.0f));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["Main Menu"];
		application->setState(GameState::NOTSTARTEDYET);
		ui["Pause Menu"]->hideMain();
		ui["Main Menu"]->showMain();
	};
	ui["Pause Menu"]->addText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(48.0f, 55.0f));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = nullptr;
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	ui["Pause Menu"]->addText(options);

	//Options
	options = std::make_shared<Text>("A", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
	options->onMouseReleaseFunc = [Options, C]()
	{
		currentMenu = C;
		Options->hideMain();
		C->showMain();
	};
	Options->addText(options);
	options = std::make_shared<Text>("B", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
	Options->addText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(50.0f, 50.0f));
	options->onMouseReleaseFunc = [Options]()
	{
		currentMenu = ui["Main Menu"];

		Options->hideMain();
		ui["Main Menu"]->showMain();
	};
	Options->addText(options);
	ui["Main Menu"]->addUIElement(Options);

	//C
	options = std::make_shared<Text>("C", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(50.0f, 60.0f));
	C->addText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(50.0f, 55.0f));
	options->onMouseReleaseFunc = [Options, C]()
	{
		currentMenu = Options;
		C->hideMain();
		Options->showMain();
	};
	C->addText(options);
	Options->addUIElement(C);

	//Game Over
	options = std::make_shared<Text>("Game Over", 32, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 0.0f), application->getFont("kenvector_future_thin"), glm::vec2(40.0f, 55.0f));
	options->setIsStatic(true);
	ui["Game Over"]->addText(options);
}

void redrawPlayerUI()
{	
	glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

	for (std::unordered_map<std::string, std::shared_ptr<UIElement>>::iterator it = playerUI.begin(); it != playerUI.end();)
	{
		if (it->first == "Score" && player->getLastScore() != player->getScore())
		{
			it = playerUI.erase(it);
			playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), nullptr, glm::vec2(0.0f, 0.0f))));
			
			//Score
			auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), application->getFont("kenvector_future_thin"), glm::vec2(90.0f, 93.0f));
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
			option2->applyTexture(std::make_shared<Texture>(*application->getTexture("playerLife1_blue")));
			playerUI["Health"]->addUIElement(option2);
			auto option = std::make_shared<Text>(" X " + std::to_string(player->getHealth()), 18, glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), application->getFont("kenvector_future_thin"), glm::vec2(12.0f, 92.0f));
			option->setIsStatic(true);
			playerUI["Health"]->addText(option);
			playerUI["Health"]->fixPosition();
			break;
		}
		else
			++it;
	}
}

void motionFunc(int x, int y)
{
	auto inputManager = application->getInputManager();
	inputManager->setLastMousePosition(glm::vec2(x, y));

	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y *= -1;

	for (auto uiElement : ui)
	{
		uiElement.second->checkIfMouseHoverThis(lastMousePosition);
	}
}

void processMouseClick(int button, int state, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y *= -1;

	auto inputManager = application->getInputManager();
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			inputManager->setLeftMouseState(true);
			inputManager->setLastLeftMouseState(false);
		}
		else
		{
			inputManager->setLeftMouseState(false);
			inputManager->setLastLeftMouseState(true);
		}

		for (auto uiElement : ui)
		{
			uiElement.second->checkForMouseClickOnThis(inputManager, lastMousePosition);
		}
	}
	/*if (button == GLUT_RIGHT_BUTTON)
		state == GLUT_UP ? inputManager->SetRightMouseState(true), inputManager->SetLastRightMouseState(false) : inputManager->SetRightMouseState(false), inputManager->SetLastRightMouseState(true);*/
}

void keyboardInput(unsigned char c, int x, int y)
{
	auto inputManager = application->getInputManager();
	if (!inputManager->getKey(c))
	{
		switch (c)
		{
			case 27:
			{
				if (application->getState() == GameState::ENDED)
				{
					currentMenu = ui["Main Menu"];
					ui["Game Over"]->hideMain();
					ui["Main Menu"]->showMain();
					application->setState(GameState::NOTSTARTEDYET);
				}
				else if (application->getState() == GameState::PAUSED)
				{
					currentMenu = nullptr;
					ui["Pause Menu"]->hideMain();
					application->setState(GameState::STARTED);
				}
				else if (application->getState() == GameState::STARTED)
				{
					currentMenu = ui["Pause Menu"];
					ui["Pause Menu"]->showMain();
					application->setState(GameState::PAUSED);
				}
				else if (application->getState() == GameState::NOTSTARTEDYET)
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
			case 32:
			{
				player->addBullet(std::make_shared<BaseGameObject>(9, 20, glm::vec2(player->getPosition(0) + (player->getSize(0) / 2.0f), player->getPosition(1) + player->getSize(1) + 5.0f), glm::vec2(0.0f, 200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f)), std::make_shared<Texture>(*application->getTexture("laserBlue01")));
			}
		}
		inputManager->setKey(c, true);
	}
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	auto inputManager = application->getInputManager();
	if (inputManager->getKey(c))
		inputManager->setKey(c, false);
}

void startNewLevel(int value)
{
	playerUI["Level completed"]->hideMain();
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	auto renderer = application->getRender();
	auto tempShader = renderer->getShaderProgram("shader");
	auto tempVAO = renderer->getVAO();
	auto tempTextShader = renderer->getShaderProgram("textshader");
	auto tempTextVAO = renderer->getTextVAO();

	auto listOfAllElements = std::vector<std::shared_ptr<UIElement>>();
	auto listOfAllTexts = std::vector<std::shared_ptr<Text>>();

	float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
	float frameTime = (newTime - currentTime) / 1000.0f;
	currentTime = newTime;

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		if (application->getState() == GameState::STARTED)
		{
			player->update(dt, application->getInputManager());
			for (auto enemy : enemies)
			{
				enemy->update(dt, t);
			}
			t += dt;
		} 
		accumulator -= dt;
	}

	if (application->getState() == GameState::STARTED)
	{
		std::vector<std::shared_ptr<BaseGameObject>> tempTestEnemyList;

		auto collisionManager = application->getCollisionManager();
		auto playerBulletList = player->getBulletsList();

		for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end();)
		{
			bool collided = false;
			auto enemyBulletList = (*it)->getBulletsList();
			auto collision = collisionManager->checkCollision(player, enemyBulletList);

			if (collision != enemyBulletList->end())
			{
				auto explosion = std::make_shared<Explosion>(32, 32, (*collision)->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
				explosion->applyTexture(std::make_shared<Texture>(*application->getTexture("blueExplosionSpriteSheet")));
				explosions.push_back(explosion);

				enemyBulletList->erase(collision);
				std::cout << "enemy bullet hit player" << std::endl;
			}

			collision = collisionManager->checkCollision(*it, playerBulletList);

			if (collision != playerBulletList->end())
			{
				collided = true;

				auto explosion = std::make_shared<Explosion>(32, 32, (*collision)->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
				explosion->applyTexture(std::make_shared<Texture>(*application->getTexture("blueExplosionSpriteSheet")));
				explosions.push_back(explosion);

				player->setScore(player->getScore() + 100);
				playerBulletList->erase(collision);
				it = enemies.erase(it);

				if (enemies.size() == 0)
				{
					glutTimerFunc(2000, startNewLevel, 0);
					playerUI["Level completed"]->showMain();
				}

				std::cout << "player bullet hit enemy; " << enemies.size() << " enemies left" << std::endl;
			}

			if (it != enemies.end() && collisionManager->checkCollision(player, *it))
			{
				player->respawn();
				std::cout << "player hit enemy" << std::endl;
			}

			if (!collided)
				it++;
		}

		if (player->getHealth() < 1)
		{
			application->setState(GameState::ENDED);
			currentMenu = ui["Game Over"];
			ui["Game Over"]->showMain();
		}

		for (std::vector<std::shared_ptr<TestEnemy>>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		{
			tempTestEnemyList.push_back(*it);
		}

		redrawPlayerUI();

		renderer->draw(background);
		renderer->draw(player);
		renderer->draw(tempTestEnemyList);

		std::vector<std::shared_ptr<BaseGameObject>> tempBaseGameObjectList;

		for (std::vector<std::shared_ptr<Explosion>>::iterator it = explosions.begin(); it != explosions.end();)
		{
			if ((*it)->update(frameTime))
				it = explosions.erase(it);
			else
			{
				tempBaseGameObjectList.push_back(*it);
				++it;
			}
		}

		renderer->draw(tempBaseGameObjectList);

		for (auto uiElement : playerUI)
		{
			uiElement.second->GetAllChildrenElements(&listOfAllElements);
			uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
		}

		for (auto element : listOfAllElements)
		{
			element->update(dt);
		}

		auto tempBaseList = std::vector<std::shared_ptr<UIElementBase>>();
		for (auto element : listOfAllElements)
			tempBaseList.push_back(element);
		renderer->draw(tempBaseList);

		for (auto text : listOfAllTexts)
		{
			text->update(dt);
		}

		renderer->draw(listOfAllTexts);

		listOfAllElements.clear();
		listOfAllTexts.clear();
	}

	for (auto uiElement : ui)
	{
		uiElement.second->GetAllChildrenElements(&listOfAllElements);
		uiElement.second->GetAllChildrenTexts(&listOfAllTexts);
	}

	for (auto element : listOfAllElements)
	{
		element->update(dt);
	}

	auto tempBaseList = std::vector<std::shared_ptr<UIElementBase>>();
	for (auto element : listOfAllElements)
		tempBaseList.push_back(element);
	renderer->draw(tempBaseList);

	for (auto text : listOfAllTexts)
	{
		text->update(dt);
	}

	renderer->draw(listOfAllTexts);
	glutSwapBuffers();
}

void resize(int width, int height)
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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 300);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Space Shooter");

	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		#if _DEBUG
			std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		#endif
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutIgnoreKeyRepeat(GL_TRUE);

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyboardInputUp);
	glutMouseFunc(processMouseClick);
	glutPassiveMotionFunc(motionFunc);

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

	application = std::make_shared<Application>();
	application->addShader("shader", std::make_shared<Shader>("shader.vert", "shader.frag"));
	application->addShader("textshader", std::make_shared<Shader>("textshader.vert", "textshader.frag"));

	application->loadFont("kenvector_future.ttf", "kenvector_future");
	application->loadFont("kenvector_future_thin.ttf", "kenvector_future_thin");

	application->loadTexture("PNG/UI/playerLife1_blue.png", "playerLife1_blue", 0, 0, glm::vec2(1,1));
	application->loadTexture("PNG/playerShip1_blue.png", "playerShip1_blue", 0, 0, glm::vec2(1, 1));
	application->loadTexture("PNG/Enemies/enemyBlack1.png", "enemyBlack1", 0, 0, glm::vec2(1, 1));
	application->loadTexture("Backgrounds/blue.png", "blueBackground", 0, 0, glm::vec2(1, 1));
	application->loadTexture("PNG/Lasers/laserBlue01.png", "laserBlue01", 0, 0, glm::vec2(1, 1));
	application->loadTexture("PNG/Lasers/blueExplosionSpriteSheet.png", "blueExplosionSpriteSheet", 0, 2, glm::vec2(2, 1));

	initGameUI();
	currentMenu = ui["Main Menu"];

	background = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), nullptr, glm::vec2(0.0f, 0.0f));
	background->applyTexture(std::make_shared<Texture>(*application->getTexture("blueBackground")));
	player = std::make_shared<Player>(32, 32, glm::vec2((float)glutGet(GLUT_WINDOW_X) / 2.0f, 0.0f), glm::vec2(80.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
	player->applyTexture(std::make_shared<Texture>(*application->getTexture("playerShip1_blue")));

	initPlayerUI();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
