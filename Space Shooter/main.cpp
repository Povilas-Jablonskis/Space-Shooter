#include "Application.h"
using namespace Engine;

std::shared_ptr<Application> application;

void motionFunc(int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->motionFunc(lastMousePosition);
}

void processMouseClick(int button, int state, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->processMouseClick(button, state, lastMousePosition);
}

void keyboardInput(unsigned char c, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->keyboardInput(c, lastMousePosition);
}

void keyboardInputUp(unsigned char c, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->keyboardInputUp(c, lastMousePosition);
}

void display(void)
{
	application->render();
}

void resize(int width, int height)
{
	application->resize(width, height);
}

void specialKeyInput(int key, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->specialKeyInput(key, lastMousePosition);
}

void specialKeyInputUp(int key, int x, int y)
{
	glm::vec2 lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
	lastMousePosition.y = std::abs(lastMousePosition.y);
	application->specialKeyInputUp(key, lastMousePosition);
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
