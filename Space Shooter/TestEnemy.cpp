#include "TestEnemy.h"

namespace Engine
{
	TestEnemy::TestEnemy()
	{
		dt = 0.0f;
	}

	TestEnemy::TestEnemy(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b) : BaseGameObject(_width, _height, x, y, velx, vely, r, g, b)
	{
		dt = 0.0f;
		float _dt = 0.0f;
		float maxpos = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		float value = 0.0f;
		while (_dt < 3.14f)
		{
			value += sin(_dt);
			_dt += 0.2f;
		}

		velocity[1] = (maxpos - position[1] - height) / value;
	}

	TestEnemy::~TestEnemy()
	{

	}

	void TestEnemy::Update()
	{
		dt += 0.2f;

		position[0] += velocity[0];
		position[1] += velocity[1] * sin(dt);
	}
}