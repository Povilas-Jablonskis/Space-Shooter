#include "Application.h"
using namespace Engine;

void motionFunc(int x, int y)
{
	Application::instance()->motionFunc(x, y);
}

void processMouseClick(int button, int state, int x, int y)
{
	Application::instance()->processMouseClick(button, state, x, y);
}

void keyboardInput(unsigned char c, int x, int y)
{
	Application::instance()->keyboardInput(c, x, y);
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	Application::instance()->keyboardInputUp(c, x, y);
}

void display(void)
{
	Application::instance()->render();
}

void resize(int width, int height)
{
	Application::instance()->resize(width, height);
}

void specialKeyInput(int key, int x, int y)
{
	Application::instance()->specialKeyInput(key, x, y);
}

void specialKeyInputUp(int key, int x, int y)
{
	Application::instance()->specialKeyInputUp(key, x, y);
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
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(specialKeyInputUp);

	Application::instance();

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
