#include "Bullet.h"
#include "Player.h"
#include "TestEnemy.h"

namespace Engine
{
	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}

	Bullet::~Bullet()
	{

	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position.y >= windowHeigth || position.y <= 0.0f || position.x >= windowWidth || position.x <= 0.0f)
			return false;
		return true;
	}

	void Bullet::onCollision(BaseGameObject* collider, BaseGameObject* parent)
	{
		Player* player = dynamic_cast<Player*>(parent);
		if (player != nullptr)
		{
			collider->setNeedsToBeDeleted(true);
			player->deleteBullet(this);
			player->setScore(player->getScore() + 100);
			std::cout << "player bullet hit" << std::endl;
		}
		TestEnemy* enemy = dynamic_cast<TestEnemy*>(parent);
		if (enemy != nullptr)
		{
			Player* player = dynamic_cast<Player*>(collider);
			if (player != nullptr)
				player->respawn();
			enemy->deleteBullet(this);
			std::cout << "enemy bullet hit" << std::endl;
		}
	}
}