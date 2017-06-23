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

	glutTimerFunc(1000 / 60, timerFunc, value);
	prevtime = currtime;
	currtime = glutGet(GLUT_ELAPSED_TIME);
	dt = (currtime - prevtime) / 1000.0f;

	if (dt > 0.016f)
		dt = 0.016f;

	player.Update();
	Renderer::Render(player);
	for (auto bullet : BulletManager::GetBulletList())
	{
		bullet->Update();
		Renderer::Render(bullet);
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
	glutCreateWindow("SideScroller");

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

	UIElement* element = new UIElement(32, 32, 50.0f / 100.0f, 50.0f / 100.0f, 255.0f, 255.0f, 0.0f, 0.0f);
	element->AddText(new Text("The Quick Brown Fox Jumps Over The Lazy Dog2", 14, 0.0f / 100.0f, 30.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf"));
	element->AddText(new Text("The Quick Brown Fox Jumps Over The Lazy Dog", 14, 0.0f / 100.0f, 90.0f / 100.0f, 255.0f, 160.0f, 122.0f, 1.0f, "AGENCYR.ttf"));

	UI.push_back(element);

	player = Player(32, 32, 320.0f, 0.0f, 0.0f, 0.5f, 255.0f, 255.0f, 0.0f);

	glutTimerFunc(1000 / 60, timerFunc, 0);
	currtime = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
