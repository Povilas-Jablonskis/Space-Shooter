#include "TestEnemy.h"
#include "Player.h"

namespace Engine
{
	TestEnemy::TestEnemy(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShoots(1.5f), delayBetweenShootsTimer(0.0f)
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
		bullets.clear();
	}

	bool TestEnemy::update(float dt, float t)
	{
		position.x += velocity.x * dt;
		position.y += velocity.y * sin(t);
		BaseGameObject::updateAnimation(dt);

		delayBetweenShootsTimer += dt;
		if (delayBetweenShootsTimer > delayBetweenShoots)
		{
			delayBetweenShootsTimer = 0.0f;
			auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(position.x + (width / 2.0f), position.y - 5.0f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
			bullet->applyAnimation(animations["shoot"]);
			bullet->setRotationAngle(3.141592f);
			bullet->setRotationAxis(glm::vec3(0.0,0.0,1.0));
			bullets.push_back(std::move(bullet));
		}

		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			if ((*it)->update(dt))
				it = bullets.erase(it);
			else
				++it;
		}

		return needsToBeDeleted;
	}

	void TestEnemy::deleteBullet(Bullet* bullet)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); it++)
		{
			if (it->get() == bullet)
			{
				(*it)->setNeedsToBeDeleted(true);
				return;
			}
		}
	}

	void TestEnemy::onCollision(Player* collider)
	{
		collider->respawn();

		#if _DEBUG
			std::cout << "enemy hit" << std::endl;
		#endif
	}
}