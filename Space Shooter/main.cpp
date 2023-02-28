#include "Application.hpp"
#include "InputManager.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <iostream>

std::shared_ptr<Application> application;

void motionFunc(const int x, const int y)
{
	application->getInputManager().motionFunc(x, y);
}

void processMouseClick(const int button, const int state, const int x, const int y)
{
	application->getInputManager().processMouseClick(button, state, x, y);
}

void keyboardInput(const unsigned char c, const int, const int)
{
	application->getInputManager().keyboardInput(static_cast<char>(c));
}

void keyboardInputUp(const unsigned char c, const int x, const int y)
{
	application->getInputManager().keyboardInputUp(static_cast<char>(c), x, y);
}

void display()
{
	application->render();
}

void resize(int, int)
{
	const auto windowWidth = glutGet(GLUT_INIT_WINDOW_WIDTH);
	const auto windowHeight = glutGet(GLUT_INIT_WINDOW_HEIGHT);

	glutReshapeWindow(windowWidth, windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, windowWidth, 0.0, windowHeight, 0.0, 1.0);
}

void specialKeyInput(const int key, const int x, const int y)
{
	application->getInputManager().specialKeyInput(key, x, y);
}

void specialKeyInputUp(const int key, const int x, const int y)
{
	application->getInputManager().specialKeyInputUp(key, x, y);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Space Shooter");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glewExperimental = true;

	const auto err = glewInit();
	if (GLEW_OK != err)
	{
#if _DEBUG
		std::cout << "ERROR::GLEW: " << glewGetErrorString(err) << "\n";
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
