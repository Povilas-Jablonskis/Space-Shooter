#include "TestEnemy.h"

namespace Engine
{
	TestEnemy::TestEnemy(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color) 
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		float _dt = 0.0f;
		float maxPos = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		float value = 0.0f;
		while (_dt < 3.14f)
		{
			value += sin(_dt);
			_dt += (1.0f / 60.0f);
		}

		velocity.y = (maxPos - position.y - height) / value;
	}

	TestEnemy::~TestEnemy()
	{

	}

	bool TestEnemy::update(float _dt, float _t)
	{
		if (texture != nullptr)
			texture->update(_dt);

		position.x += velocity.x * _dt;
		position.y += velocity.y * sin(_t);
		return true;
	}

	void TestEnemy::onCollision(BaseGameObject* collider)
	{
		std::cout << "enemy hit" << std::endl;
	}
}