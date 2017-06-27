#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <map>

#include "BaseGameObject.h"
#include "Shader.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "TestEnemy.h"
#include "UIElement.h"
#include "UIElementBase.h"
#include "Text.h"
#include "Renderer.h"

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

using namespace Engine;

int prevtime, currtime;
float dt;

Player player;

std::vector<BaseGameObject*> enemies;
std::vector<UIElement*> UI;

void idle(void)
{
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

void timerFunc(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutTimerFunc(1000 / 30, timerFunc, value);
	prevtime = currtime;
	currtime = glutGet(GLUT_ELAPSED_TIME);
	dt = (currtime - prevtime) / 1000.0f;

	if (dt > 0.033f)
		dt = 0.033f;

	player.Update();
	Renderer::Render(&player);
	auto list = BulletManager::GetBulletList();
	for (std::vector<std::shared_ptr<Bullet>>::iterator it = list->begin(); it != list->end();)
	{
		auto bullet = it->get();
		if (bullet->UpdateBullet())
			it = list->erase(it);
		else
		{
			Renderer::Render(bullet);
			++it;
		}
		
	}
	for (auto enemy : enemies)
	{
		enemy->Update();
		Renderer::Render(enemy);
	}
	for (auto UIElement : UI)
	{
		UIElement->Update();
		Renderer::Render(UIElement);
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

	FontLoader::Init();
	Renderer::Init();
	Renderer::AddShader("shader", new Shader("shader.vert", "shader.frag"));
	Renderer::AddShader("textshader", new Shader("textshader.vert", "textshader.frag"));

	for (size_t i = 0; i <= 16; i++)
	{
		BaseGameObject* enemy = new TestEnemy(32, 32, 32.0f + (i * 34.0f), 416.0f, 0.0f, 0.0f, 255.0f, 160.0f, 122.0f);
		enemies.push_back(enemy);
	}

	UIElement* mainmenu = new UIElement(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f / 100.0f, 0.0f / 100.0f, 255.0f, 255.0f, 0.0f, 0.0f);
	UIElement* optionsUI = new UIElement(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0f / 100.0f, 0.0f / 100.0f, 255.0f, 255.0f, 0.0f, 0.0f);
	
	auto text = new Text("Start Game", 18, 50.0f / 100.0f, 60.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	mainmenu->AddText(text);
	text = new Text("Options", 18, 50.0f / 100.0f, 55.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	text->OnMouseReleaseFunc = [optionsUI, mainmenu]()
	{
		mainmenu->HideAllElements();
		optionsUI->ShowAllElements();
	};
	mainmenu->AddText(text);
	text = new Text("End Game", 18, 50.0f / 100.0f, 50.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	text->OnMouseReleaseFunc = []()
	{
		std::cout << "exiting" << std::endl;
		getchar();
		exit(0);
	};
	mainmenu->AddText(text);
	UI.push_back(mainmenu);

	auto options = new Text("A", 18, 50.0f / 100.0f, 60.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	optionsUI->AddText(options);
	options = new Text("B", 18, 50.0f / 100.0f, 55.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	optionsUI->AddText(options);
	options = new Text("Back", 18, 50.0f / 100.0f, 50.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf");
	options->OnMouseReleaseFunc = [optionsUI, mainmenu]()
	{
		optionsUI->HideAllElements();
		mainmenu->ShowAllElements();
	};
	optionsUI->AddText(options);
	optionsUI->HideAllElements();
	UI.push_back(optionsUI);

	player = Player(32, 32, 320.0f, 0.0f, 0.0f, 0.5f, 255.0f, 255.0f, 0.0f);

	glutTimerFunc(1000 / 30, timerFunc, 0);
	currtime = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
