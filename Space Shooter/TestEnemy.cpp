#include "TestEnemy.h"

namespace Engine
{
	TestEnemy::TestEnemy()
	{

	}

	TestEnemy::TestEnemy(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color) 
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		float _dt = 0.0f;
		float maxpos = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		float value = 0.0f;
		while (_dt < 3.14f)
		{
			value += sin(_dt);
			_dt += (1.0f / 60.0f);
		}

		velocity[1] = (maxpos - position[1] - height) / value;
	}

	TestEnemy::~TestEnemy()
	{

	}

	bool TestEnemy::Update(float _dt, float _t)
	{
		position[0] += velocity[0] * _dt;
		position[1] += velocity[1] * sin(_t);
		return true;
	}

	GLboolean TestEnemy::CheckCollision(std::shared_ptr<BaseGameObject> _objecttocheck) // AABB - AABB collision
	{
		if (BaseGameObject::CheckCollision(_objecttocheck))
		{
			OnCollision(_objecttocheck);
			return true;
		}
		for (auto bullet : bullets)
		{
			if (bullet->CheckCollision(_objecttocheck))
				return true;
		}
		return false;
	}

	GLboolean TestEnemy::CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck) // AABB - AABB collision
	{
		for (auto object : *_objectstocheck)
		{
			if (BaseGameObject::CheckCollision(object))
			{
				OnCollision(object);
				return true;
			}
		}
		for (auto bullet : bullets)
		{
			for (auto object : *_objectstocheck)
			{
				if (bullet->CheckCollision(object))
					return true;
			}
		}
		return false;
	}

	void TestEnemy::OnCollision(std::shared_ptr<BaseGameObject> collider)
	{
		std::cout << "enemy hit" << std::endl;
	}
}