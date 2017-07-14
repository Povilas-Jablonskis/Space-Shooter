#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <map>
#include <string>

#include "BaseGameObject.h"
#include "Shader.h"
#include "Player.h"
#include "Bullet.h"
#include "TestEnemy.h"
#include "UIElement.h"
#include "UIElementBase.h"
#include "Text.h"
#include "GameState.h"
#include "Application.h"
#include "BulletManager.h"

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace Engine;

Application* application;
std::shared_ptr<Player> player;
BulletManager* bulletManager;

UIElement* currentMenu;

float t;
float dt;
float currentTime;
float accumulator;

std::vector<std::shared_ptr<BaseGameObject>> enemies;
std::map<std::string, std::shared_ptr<UIElement>> ui;
std::map<std::string, std::shared_ptr<UIElement>> playerUI;

void initScene()
{
	enemies.clear();
	for (size_t i = 0; i <= 16; i++)
	{
		auto enemy = std::make_shared<TestEnemy>(32, 32, glm::vec2(32.0f + (i * 34.0f), 416.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
		enemies.push_back(enemy);
	}

	if (player == nullptr)
		player = std::make_shared<Player>(32, 32, glm::vec2(320.0f, 0.0f), glm::vec2(50.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));
	else
	{
		player->setPosition(glm::vec2(320.0f, 0.0f));
		player->setHealth(player->getHealth() - 1);
	}

	accumulator = 0.0f;
	t = 0.0f;
}

void initGameUI()
{
	glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Options", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), ui["Main Menu"].get())));
	ui.insert(std::pair<std::string, std::shared_ptr<UIElement>>("A", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f), ui["Options"].get())));

	//Main Menu
	auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(temPos.x * (48.0f / 100.0f), temPos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		application->setState(GameState::STARTED);
		currentMenu = nullptr;
		ui["Main Menu"]->hideAllElements();
		initScene();
	};
	ui["Main Menu"]->addText(options);
	options = std::make_shared<Text>("Options", 18, glm::vec2(temPos.x * (48.0f / 100.0f), temPos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["Options"].get();
		ui["Main Menu"]->hideAllElements();
		ui["Options"]->showAllElements();
	};
	ui["Main Menu"]->addText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(temPos.x * (48.0f / 100.0f), temPos.y * (50.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = nullptr;
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	ui["Main Menu"]->addText(options);

	//Pause Menu
	options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(temPos.x * (45.0f / 100.0f), temPos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["Main Menu"].get();
		application->setState(GameState::NOTSTARTEDYET);
		ui["Pause Menu"]->hideAllElements();
		ui["Main Menu"]->showAllElements();
	};
	ui["Pause Menu"]->addText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(temPos.x * (48.0f / 100.0f), temPos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = nullptr;
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	ui["Pause Menu"]->addText(options);
	ui["Pause Menu"]->hideAllElements();

	//Options
	options = std::make_shared<Text>("A", 18, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["A"].get();
		ui["Options"]->hideAllElements();
		ui["A"]->showAllElements();
	};
	ui["Options"]->addText(options);
	options = std::make_shared<Text>("B", 18, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	ui["Options"]->addText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (50.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["Main Menu"].get();
		ui["Options"]->hideAllElements();
		ui["Main Menu"]->showAllElements();
	};
	ui["Options"]->addText(options);
	ui["Options"]->hideAllElements();

	//A
	options = std::make_shared<Text>("A", 18, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	ui["A"]->addText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	options->onMouseReleaseFunc = []()
	{
		currentMenu = ui["Options"].get();
		ui["A"]->hideAllElements();
		ui["Options"]->showAllElements();
	};
	ui["A"]->addText(options);
	ui["A"]->hideAllElements();

	//Game Over
	options = std::make_shared<Text>("Game Over", 32, glm::vec2(temPos.x * (45.0f / 100.0f), temPos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")), true);
	ui["Game Over"]->addText(options);
	ui["Game Over"]->hideAllElements();
}

void redrawPlayerUI()
{
	playerUI.clear();

	glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

	playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	playerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));

	//scoreUI
	auto option = std::make_shared<Text>(std::to_string(player->getScore()), 18, glm::vec2(temPos.x * (6.0f / 100.0f), temPos.y * (92.0f / 100.0f)), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), ((FT_Face*)application->getFont("AGENCYR.ttf")));
	playerUI["Score"]->addText(option);

	//healthUI
	auto option2 = std::make_shared<UIElement>(196, 32, glm::vec2(temPos.x * (50.0f / 100.0f), temPos.y * (90.0f / 100.0f)), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f));
	option2->applyTexture(application->getTexture("healthBar"));
	option2->setCurrentTextureCurrentFrame(player->getHealth());
	playerUI["Health"]->addUIElement(option2);
}

void motionFunc(int x, int y)
{
	auto inputManager = &application->getInputManager();
	inputManager->setLastMousePosition(glm::vec2(x, y));
}

void processMouseClick(int button, int state, int x, int y)
{
	auto inputManager = &application->getInputManager();
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			inputManager->setLeftMouseState(true);
			inputManager->setLastLeftMouseState(false);
			for (auto uiElement : ui)
			{
				uiElement.second->onMouseClickDefaults(inputManager);
			}
		}
		else
		{
			inputManager->setLeftMouseState(false);
			inputManager->setLastLeftMouseState(true);
			for (auto uiElement : ui)
			{
				uiElement.second->onMouseReleaseFuncDefaults(inputManager);
			}
		}
	}
	/*if (button == GLUT_RIGHT_BUTTON)
		state == GLUT_UP ? inputManager->SetRightMouseState(true), inputManager->SetLastRightMouseState(false) : inputManager->SetRightMouseState(false), inputManager->SetLastRightMouseState(true);*/
}

void keyboardInput(unsigned char c, int x, int y)
{
	auto inputManager = &application->getInputManager();
	if (!inputManager->getKey(c))
	{
		switch (c)
		{
			case 27:
			{
				if (application->getState() == GameState::ENDED)
				{
					currentMenu = ui["Main Menu"].get();
					ui["Game Over"]->hideAllElements();
					ui["Main Menu"]->showAllElements();
					application->setState(GameState::NOTSTARTEDYET);
				}
				else if (application->getState() == GameState::PAUSED)
				{
					currentMenu = nullptr;
					ui["Pause Menu"]->hideAllElements();
					application->setState(GameState::STARTED);
				}
				else if (application->getState() == GameState::STARTED)
				{
					currentMenu = ui["Pause Menu"].get();
					ui["Pause Menu"]->showAllElements();
					application->setState(GameState::PAUSED);
				}
				else if (application->getState() == GameState::NOTSTARTEDYET)
				{
					if (currentMenu != nullptr && currentMenu->getParent() != nullptr)
					{
						currentMenu->hideAllElements();
						currentMenu = currentMenu->getParent();
						currentMenu->showAllElements();
					}
				}
				break;
			}
		}
		inputManager->setKey(c, true);
	}
	else
		inputManager->setKey(c, false);
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	auto inputManager = &application->getInputManager();
	if (inputManager->getKey(c))
		inputManager->setKey(c, false);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	auto inputManager = &application->getInputManager();

	float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
	float frameTime = (newTime - currentTime) / 1000.0f;
	currentTime = newTime;

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		if (application->getState() == GameState::STARTED)
		{
			player->update(inputManager, dt, bulletManager->GetBulletList());
			for (auto enemy : enemies)
			{
				enemy->update(dt, t);
			}
			bulletManager->updateBulletList(dt);
		}
		accumulator -= dt;
		t += dt;
	}

	if (application->getState() == GameState::STARTED)
	{
		bulletManager->drawBulletList();
		if (player->checkCollision(&enemies))
		{
			application->setState(GameState::ENDED);
			currentMenu = ui["Game Over"].get();
			ui["Game Over"]->showAllElements();
		}
		bulletManager->checkCollision(player);
		bulletManager->checkCollision(&enemies);

		player->draw();
		redrawPlayerUI();

		for (auto enemy : enemies)
		{
			enemy->draw();
		}

		for (auto uiElement : playerUI)
		{
			uiElement.second->update(inputManager, dt);
			uiElement.second->draw();
		}
	}

	for (auto uiElement : ui)
	{
		uiElement.second->update(inputManager, dt);
		uiElement.second->draw();
	}

	glutSwapBuffers();
}

void resize(int width, int height)
{
	//const float ar = (float) width / (float) height;

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

	glutIgnoreKeyRepeat(1);

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyboardInput);
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

	bulletManager = new BulletManager();
	application = new Application();
	application->addShader("shader", new Shader("shader.vert", "shader.frag"));
	application->addShader("textshader", new Shader("textshader.vert", "textshader.frag"));

	application->loadFont("AGENCYR.ttf", "AGENCYR.ttf");

	application->loadTexture("GUI/healthBar.png", "healthBar", glm::vec2(1,6));

	initGameUI();
	currentMenu = ui["Main Menu"].get();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
