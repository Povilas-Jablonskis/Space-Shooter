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

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace Engine;

Application* application;
std::shared_ptr<Player> player;

float t;
float dt;
float currentTime;
float accumulator;

std::vector<std::shared_ptr<BaseGameObject>> enemies;
std::map<std::string, std::shared_ptr<UIElement>> UI;
std::map<std::string, std::shared_ptr<UIElement>> PlayerUI;

void InitScene()
{
	enemies.clear();
	for (size_t i = 0; i <= 16; i++)
	{
		auto enemy = std::make_shared<TestEnemy>(32, 32, glm::vec2(32.0f + (i * 34.0f), 416.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
		enemies.push_back(enemy);
	}

	player = std::make_shared<Player>(32, 32, glm::vec2(320.0f, 0.0f), glm::vec2(50.0f, 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 1.0f));

	accumulator = 0.0f;
	t = 0.0f;
}

void InitGameUI()
{
	glm::vec2 tempos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));
	UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Main Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Pause Menu", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Options", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	UI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Game Over", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f / 100.0f, 0.0f / 100.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));

	//Main Menu
	auto options = std::make_shared<Text>("Start Game", 18, glm::vec2(tempos.x * (48.0f / 100.0f), tempos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		application->SetState(GameState::STARTED);
		UI["Main Menu"]->HideAllElements();
		InitScene();
	};
	UI["Main Menu"]->AddText(options);
	options = std::make_shared<Text>("Options", 18, glm::vec2(tempos.x * (48.0f / 100.0f), tempos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		UI["Main Menu"]->HideAllElements();
		UI["Options"]->ShowAllElements();
	};
	UI["Main Menu"]->AddText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(tempos.x * (48.0f / 100.0f), tempos.y * (50.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	UI["Main Menu"]->AddText(options);

	//Pause Menu
	options = std::make_shared<Text>("Go To Main Menu", 18, glm::vec2(tempos.x * (45.0f / 100.0f), tempos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		InitScene();
		application->SetState(GameState::NOTSTARTEDYET);
		UI["Pause Menu"]->HideAllElements();
		UI["Main Menu"]->ShowAllElements();
	};
	UI["Pause Menu"]->AddText(options);
	options = std::make_shared<Text>("End Game", 18, glm::vec2(tempos.x * (48.0f / 100.0f), tempos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	UI["Pause Menu"]->AddText(options);
	UI["Pause Menu"]->HideAllElements();

	//Options
	options = std::make_shared<Text>("A", 18, glm::vec2(tempos.x * (50.0f / 100.0f), tempos.y * (60.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	UI["Options"]->AddText(options);
	options = std::make_shared<Text>("B", 18, glm::vec2(tempos.x * (50.0f / 100.0f), tempos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	UI["Options"]->AddText(options);
	options = std::make_shared<Text>("Back", 18, glm::vec2(tempos.x * (50.0f / 100.0f), tempos.y * (50.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	options->OnMouseReleaseFunc = []()
	{
		UI["Options"]->HideAllElements();
		UI["Main Menu"]->ShowAllElements();
	};
	UI["Options"]->AddText(options);
	UI["Options"]->HideAllElements();

	//Game Over
	options = std::make_shared<Text>("Game Over", 32, glm::vec2(tempos.x * (45.0f / 100.0f), tempos.y * (55.0f / 100.0f)), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")), true);
	UI["Game Over"]->AddText(options);
	UI["Game Over"]->HideAllElements();
}

void InitPlayerUI()
{
	PlayerUI.clear();

	glm::vec2 tempos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

	PlayerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Score", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));
	PlayerUI.insert(std::pair<std::string, std::shared_ptr<UIElement>>("Health", std::make_shared<UIElement>(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 0.0f, 0.0f))));

	//scoreUI
	auto option = std::make_shared<Text>(std::to_string(player->GetScore()), 18, glm::vec2(tempos.x * (6.0f / 100.0f), tempos.y * (92.0f / 100.0f)), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
	PlayerUI["Score"]->AddText(option);

	//healthUI
	for (int i = 0; i < player->GetHealth(); i++)
	{
		auto option2 = std::make_shared<UIElement>(32, 32, glm::vec2(tempos.x * ((80 + (i * 6)) / 100.0f), tempos.y * (90.0f / 100.0f)), glm::vec4(178.0f, 34.0f, 34.0f, 1.0f));
		PlayerUI["Health"]->AddUIElement(option2);

		option = std::make_shared<Text>(std::to_string(i), 18, glm::vec2(option2.get()->GetPosition(0) + (option2.get()->GetSize(0) * (40.0f / 100.0f)), option2.get()->GetPosition(1) + (option2.get()->GetSize(1) * (37.0f / 100.0f))), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), ((FT_Face*)application->GetFont("AGENCYR.ttf")));
		option2->AddText(option);
	}
}

void motionFunc(int x, int y)
{
	auto inputManager = &application->GetInputManager();
	inputManager->SetLastMousePosition(glm::vec2(x, y));
}

void processMouseClick(int button, int state, int x, int y)
{
	auto inputManager = &application->GetInputManager();
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			inputManager->SetLeftMouseState(true);
			inputManager->SetLastLeftMouseState(false);
			for (auto UIElement : UI)
			{
				UIElement.second->OnMouseClickDefaults(inputManager);
			}
		}
		else
		{
			inputManager->SetLeftMouseState(false);
			inputManager->SetLastLeftMouseState(true);
			for (auto UIElement : UI)
			{
				UIElement.second->OnMouseReleaseFuncDefaults(inputManager);
			}
		}
	}
	/*if (button == GLUT_RIGHT_BUTTON)
		state == GLUT_UP ? inputManager->SetRightMouseState(true), inputManager->SetLastRightMouseState(false) : inputManager->SetRightMouseState(false), inputManager->SetLastRightMouseState(true);*/
}

void keyboardInput(unsigned char c, int x, int y)
{
	auto inputManager = &application->GetInputManager();
	if (!inputManager->GetKey(c))
	{
		switch (c)
		{
			case 27:
			{
				if (application->GetState() == GameState::ENDED)
				{
					UI["Game Over"]->HideAllElements();
					UI["Main Menu"]->ShowAllElements();
					application->SetState(GameState::NOTSTARTEDYET);
				}
				else if (application->GetState() == GameState::PAUSED)
				{
					UI["Pause Menu"]->HideAllElements();
					application->SetState(GameState::STARTED);
				}
				else if (application->GetState() == GameState::STARTED)
				{
					UI["Pause Menu"]->ShowAllElements();
					application->SetState(GameState::PAUSED);
				}
				break;
			}
		}
		inputManager->SetKey(c, true);
	}
	else
		inputManager->SetKey(c, false);
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	auto inputManager = &application->GetInputManager();
	if (inputManager->GetKey(c))
		inputManager->SetKey(c, false);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	auto VAO = application->GetVAO();
	auto shader = application->GetShaderProgram("shader");
	auto textshader = application->GetShaderProgram("textshader");

	auto inputManager = &application->GetInputManager();

	float newTime = ((float)glutGet(GLUT_ELAPSED_TIME));
	float frameTime = (newTime - currentTime) / 1000.0f;
	currentTime = newTime;

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		if (application->GetState() == GameState::STARTED)
		{
			player->Update(inputManager, dt);
			for (auto enemy : enemies)
			{
				enemy->Update(dt, t);
			}
		}
		accumulator -= dt;
		t += dt;
	}

	if (application->GetState() == GameState::STARTED)
	{
		if (player->CheckCollision(&enemies))
		{
			application->SetState(GameState::ENDED);
			UI["Game Over"]->ShowAllElements();
		}

		player->Draw();
		InitPlayerUI();

		for (auto enemy : enemies)
		{
			enemy->Draw();
		}

		for (auto UIElement : PlayerUI)
		{
			UIElement.second->Draw(inputManager);
		}
	}

	for (auto UIElement : UI)
	{
		UIElement.second->Update(inputManager);
		UIElement.second->Draw(inputManager);
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

	application = new Application();
	application->AddShader("shader", new Shader("shader.vert", "shader.frag"));
	application->AddShader("textshader", new Shader("textshader.vert", "textshader.frag"));
	application->LoadFont("AGENCYR.ttf", "AGENCYR.ttf");

	InitGameUI();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
