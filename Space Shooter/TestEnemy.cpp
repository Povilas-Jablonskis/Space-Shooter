#include "TestEnemy.h"
#include "Player.h"

namespace Engine
{
	TestEnemy::TestEnemy(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, std::shared_ptr<Application> _application)
		: BaseGameObject(_width, _height, _position, _velocity, _color, _application)
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
		position.x += velocity.x * _dt;
		position.y += velocity.y * sin(_t);
		BaseGameObject::updateTexture(_dt);
		return true;
	}

	void TestEnemy::onCollision(BaseGameObject* collider)
	{
		Player* tempPlayer = dynamic_cast<Player*>(collider);
		if (tempPlayer != nullptr)
		{
			tempPlayer->reset();
			tempPlayer->setHealth(tempPlayer->getHealth() - 1);
		}

		std::cout << "enemy hit" << std::endl;
	}
}