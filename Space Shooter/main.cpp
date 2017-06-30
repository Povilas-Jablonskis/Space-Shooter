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
#include "BulletManager.h"
#include "TestEnemy.h"
#include "UIElement.h"
#include "UIElementBase.h"
#include "Text.h"
#include "GameState.h"
#include "Application.h"

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace Engine;

Application* application;

int prevtime, currtime;
float dt;
bool pressedkeys[256];
GameState gamestate = GameState::NOTSTARTEDYET;

FT_Library library;
Player* player;

std::vector<BaseGameObject*> enemies;
std::map<std::string, UIElement*> UI;
std::map<std::string, std::shared_ptr<UIElement>> PlayerUI;
std::map<std::string, FT_Face> fonts;

void idle(void)
{
}

void ChangeState(GameState state)
{
	gamestate = state;
}

void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);

	//DrawContent();

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

FT_Face LoadFont(const std::string& _path, const std::string& _name)
{
	if (fonts.find(_name) != fonts.end())
		return fonts.at(_name);

	FT_Face face;
	if (FT_New_Face(library, _path.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	fonts.insert(std::pair<std::string, FT_Face>(_name, face));
	return face;
}

void InitUI()
{
	PlayerUI.clear();
	auto scoreUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));
	auto healthUI = std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));

	//scoreUI
	scoreUI->AddText(std::to_string(player->GetScore()), 18, glm::vec2(6.0f / 100.0f, 92.0f / 100.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), &fonts["AGENCYR.ttf"]);
	PlayerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", scoreUI));

	//healthUI
	for (int i = 0; i < player->GetHealth(); i++)
	{
		healthUI->AddUIElement(32, 32, glm::vec2((80 + (i * 6)) / 100.0f, 90.0f / 100.0f), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f));
	}
	PlayerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", healthUI));
}

void timerFunc(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutTimerFunc(1000 / 30, timerFunc, value);
	prevtime = currtime;
	currtime = glutGet(GLUT_ELAPSED_TIME);
	dt = (currtime - prevtime) / 1000.0f;

	if (dt > 0.033f)
		dt = 0.033f;

	SHORT escape = GetAsyncKeyState(0x1B);

	auto VAO = application->GetVAO();
	auto shader = application->GetShaderProgram("shader");
	auto textshader = application->GetShaderProgram("textshader");

	if (escape < 0 && !pressedkeys[27])
	{
		pressedkeys[27] = true;
		if (gamestate == GameState::PAUSED)
		{
			UI["Pause Menu"]->HideAllElements();
			gamestate = GameState::STARTED;
		}
		else if (gamestate != GameState::NOTSTARTEDYET)
		{
			UI["Pause Menu"]->ShowAllElements();
			gamestate = GameState::PAUSED;
		}
	}
	else if (escape == 0 && pressedkeys[27])
		pressedkeys[27] = false;

	if (gamestate == GameState::STARTED)
	{
		auto bulletslist = application->GetBulletsList();
		for (std::vector<std::shared_ptr<Bullet>> ::iterator it = bulletslist->begin(); it != bulletslist->end();)
		{
			if (it->get()->Update() == false)
				it = bulletslist->erase(it);
			else
			{
				(it->get())->Draw();
				++it;
			}
		}
		player->Update(bulletslist);
		InitUI();
		for (auto UIElement : PlayerUI)
		{
			UIElement.second->Draw();
		}
		player->Draw();
		for (auto enemy : enemies)
		{
			enemy->Update();
			enemy->Draw();
		}
	}
	for (auto UIElement : UI)
	{
		UIElement.second->Update();
		UIElement.second->Draw();
	}

	glutSwapBuffers();
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

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

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

	if (FT_Init_FreeType(&library))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	application = new Application();
	auto font = LoadFont("AGENCYR.ttf", "AGENCYR.ttf");

	for (size_t i = 0; i <= 16; i++)
	{
		BaseGameObject* enemy = new TestEnemy(32, 32, glm::vec2(32.0f + (i * 34.0f), 416.0f), glm::vec2(0.0f, 0.0f), glm::vec3(255.0f, 160.0f, 122.0f));
		enemies.push_back(enemy);
	}

	player = new Player(32, 32, glm::vec2(320.0f, 0.0f), glm::vec2(6.0f, 2.0f), glm::vec3(255.0f, 255.0f, 0.0f));

	UIElement* mainmenu = new UIElement(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));
	UIElement* optionsUI = new UIElement(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));
	UIElement* pausemenu = new UIElement(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f));

	//Main Menu
	auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(48.0f / 100.0f, 60.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = [mainmenu]()
	{
		ChangeState(GameState::STARTED);
		mainmenu->HideAllElements();
	};
	mainmenu->AddText(options);
	options = std::make_shared<Text>("Options", 18, glm::vec2(48.0f / 100.0f, 55.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = [optionsUI, mainmenu]()
	{
		mainmenu->HideAllElements();
		optionsUI->ShowAllElements();
	};
	mainmenu->AddText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(48.0f / 100.0f, 50.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = []()
	{
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	mainmenu->AddText(options);
	UI.insert(std::pair<std::string, UIElement*>("Main Menu", mainmenu));

	//Pause Menu
	options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(45.0f / 100.0f, 60.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = [mainmenu, pausemenu]()
	{
		ChangeState(GameState::NOTSTARTEDYET);
		pausemenu->HideAllElements();
		mainmenu->ShowAllElements();
	};
	pausemenu->AddText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(48.0f / 100.0f, 55.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = []()
	{
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	pausemenu->AddText(options);
	pausemenu->HideAllElements();
	UI.insert(std::pair<std::string, UIElement*>("Pause Menu", pausemenu));

	//Options
	options = std::make_shared<Text>("A", 18, glm::vec2(50.0f / 100.0f, 60.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	optionsUI->AddText(options);
	options = std::make_shared<Text>("B", 18, glm::vec2(50.0f / 100.0f, 55.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	optionsUI->AddText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(50.0f / 100.0f, 50.0f / 100.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), &font);
	options->OnMouseReleaseFunc = [optionsUI, mainmenu]()
	{
		optionsUI->HideAllElements();
		mainmenu->ShowAllElements();
	};
	optionsUI->AddText(options);
	optionsUI->HideAllElements();
	UI.insert(std::pair<std::string, UIElement*>("Options", optionsUI));

	glutTimerFunc(1000 / 30, timerFunc, 0);
	currtime = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
