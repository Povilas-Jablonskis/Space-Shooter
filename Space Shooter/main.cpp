#include "Application.h"
using namespace Engine;

std::shared_ptr<Application> application;

void motionFunc(int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->getInputManager()->setLastMousePosition(lastMousePosition);
}

void processMouseClick(int button, int state, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);

	if (button == GLUT_LEFT_BUTTON)
	{
		application->getInputManager()->setLeftMouseState(state == GLUT_DOWN ? true : false);
		application->getInputManager()->setLastMousePosition(lastMousePosition);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		application->getInputManager()->setRightMouseState(state == GLUT_DOWN ? true : false);
		application->getInputManager()->setLastMousePosition(lastMousePosition);
	}
}

void keyboardInput(unsigned char c, int x, int y)
{
	auto key = VkKeyScan(c);
	if (!application->getInputManager()->getKey(key))
	{
		switch (c)
		{
			case 27:
			{
				if (application->getGameState() == GameState::ENDED)
				{
					application->initGameMenu();
					application->setGameState(GameState::IN_MENU);
				}
				else if (application->getGameState() == GameState::IN_PAUSED_MENU)
				{
					application->getMenuManager()->getMenus()->clear();
					application->setGameState(GameState::STARTED);
				}
				else if (application->getGameState() == GameState::STARTED)
				{
					application->getMenuManager()->getMenus()->clear();

					auto pauseMenu = std::make_shared<menu>("Pause Menu", std::make_shared<Menu>());

					//Pause Menu
					auto option = std::make_shared<Text>("Go To Main Menu", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 60.0f), std::make_shared<UIInputComponent>());
					option->onMouseReleaseFunc = []()
					{
						application->getSoundEngine()->play2D("Sounds/buttonselect/3.wav", GL_FALSE);

						application->initGameMenu();
						application->setGameState(GameState::IN_MENU);
					};
					pauseMenu->second->addText(std::move(option));

					option = std::make_shared<Text>("End Game", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(48.0f, 57.0f), std::make_shared<UIInputComponent>());
					option->onMouseReleaseFunc = []()
					{
						#if _DEBUG
							std::cout << "exiting" << std::endl;
							getchar();
						#endif
						exit(0);
					};
					pauseMenu->second->addText(std::move(option));

					application->getMenuManager()->addMenu(std::move(pauseMenu));
					application->setGameState(GameState::IN_PAUSED_MENU);
				}
				else if (application->getGameState() == GameState::IN_MENU)
				{
					if (!application->getInputManager()->getCurrentlyEditedKeyBinding().empty()) break;

					auto menus = application->getMenuManager()->getMenus();
					if (menus->size() > 1)
					{
						application->getSoundEngine()->play2D("Sounds/buttonselect/5.wav", GL_FALSE);
						menus->pop_back();
					}
				}
				break;
			}
		}

		application->getInputManager()->setKey(key, true);
		application->getInputManager()->setLastKey(key, false);
	}
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	auto key = VkKeyScan(c);
	if (application->getInputManager()->getKey(key))
	{
		application->getInputManager()->setKey(key, false);
		application->getInputManager()->setLastKey(key, true);
	}
}

void display(void)
{
	application->render();
}

void resize(int width, int height)
{
	//const auto ar = (float) width / (float) height;

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

void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
		{
			if (application->getInputManager()->getKey(VK_UP))
			{
				application->getInputManager()->setKey(VK_UP, false);
				application->getInputManager()->setLastKey(VK_UP, true);
			}
			break;
		}
		case GLUT_KEY_DOWN:
		{
			if (application->getInputManager()->getKey(VK_DOWN))
			{
				application->getInputManager()->setKey(VK_DOWN, false);
				application->getInputManager()->setLastKey(VK_DOWN, true);
			}
			break;
		}
		case GLUT_KEY_LEFT:
		{
			if (application->getInputManager()->getKey(VK_LEFT))
			{
				application->getInputManager()->setKey(VK_LEFT, false);
				application->getInputManager()->setLastKey(VK_LEFT, true);
			}
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			if (application->getInputManager()->getKey(VK_RIGHT))
			{
				application->getInputManager()->setKey(VK_RIGHT, false);
				application->getInputManager()->setLastKey(VK_RIGHT, true);
			}
			break;
		}
	}

}

void specialKeyInputUp(int key, int x, int y)
{
	auto c = 0;
	switch (key)
	{
		case GLUT_KEY_UP:
		{
			c = VK_UP;
			if (!application->getInputManager()->getKey(VK_UP))
			{
				application->getInputManager()->setKey(VK_UP, true);
				application->getInputManager()->setLastKey(VK_UP, false);
			}
			break;
		}
		case GLUT_KEY_DOWN:
		{
			c = VK_DOWN;
			if (!application->getInputManager()->getKey(VK_DOWN))
			{
				application->getInputManager()->setKey(VK_DOWN, true);
				application->getInputManager()->setLastKey(VK_DOWN, false);
			}
			break;
		}
		case GLUT_KEY_LEFT:
		{
			c = VK_LEFT;
			if (!application->getInputManager()->getKey(VK_LEFT))
			{
				application->getInputManager()->setKey(VK_LEFT, true);
				application->getInputManager()->setLastKey(VK_LEFT, false);
			}
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			c = VK_RIGHT;
			if (!application->getInputManager()->getKey(VK_RIGHT))
			{
				application->getInputManager()->setKey(VK_RIGHT, true);
				application->getInputManager()->setLastKey(VK_RIGHT, false);
			}
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Space Shooter");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glewExperimental = true;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		#if _DEBUG
			std::cout << "ERROR::GLEW: " << glewGetErrorString(err) << std::endl;
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
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(specialKeyInputUp);

	application = std::make_shared<Application>();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
