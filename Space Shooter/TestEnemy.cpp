#include "TestEnemy.h"

namespace Engine
{
	TestEnemy::TestEnemy()
	{

	}

	TestEnemy::TestEnemy(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color) 
		: BaseGameObject(_width, _height, _position, _velocity, _color), dt(0.0f)
	{
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

	bool TestEnemy::Update()
	{
		dt += 0.2f;

		position[0] += velocity[0];
		position[1] += velocity[1] * sin(dt);
		return true;
	}
}