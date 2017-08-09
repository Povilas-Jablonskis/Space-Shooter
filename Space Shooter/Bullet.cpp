#include "Bullet.h"
#include "Player.h"
#include "TestEnemy.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (needsToBeDeleted || position.y > windowHeigth || (position.y + height) < 0.0f || position.x > windowWidth || position.x < 0.0f)
			return true;
		return needsToBeDeleted;
	}

	void Bullet::onCollision(BaseGameObject* collider, BaseGameObject* parent)
	{
		Player* player = dynamic_cast<Player*>(parent);
		if (player != nullptr)
		{
			collider->setNeedsToBeDeleted(true);
			player->deleteBullet(this);
			player->setScore(player->getScore() + 100);
			#if _DEBUG
				std::cout << "player bullet hit" << std::endl;
			#endif
		}
		TestEnemy* enemy = dynamic_cast<TestEnemy*>(parent);
		if (enemy != nullptr)
		{
			Player* player = dynamic_cast<Player*>(collider);
			if (player != nullptr)
			{
				if (player->getAddon("shield") != nullptr)
					player->removeAddon("shield");
				else
					player->respawn();
			}
			enemy->deleteBullet(this);
			#if _DEBUG
				std::cout << "enemy bullet hit" << std::endl;
			#endif
		}
	}
}