#include "Bullet.h"
#include "Application.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}

	void Bullet::onCollision(Player* collider, TestEnemy* parent)
	{
		parent->deleteBullet(this);
		if (collider->getAddon("shield") != nullptr)
			collider->removeAddon("shield");
		else
			collider->respawn();
		#if _DEBUG
			std::cout << "enemy bullet hit" << std::endl;
		#endif
	}

	void Bullet::onCollision(TestEnemy* collider, Player* parent)
	{
		collider->setNeedsToBeDeleted(true);
		parent->deleteBullet(this);
		parent->setScore(parent->getScore() + 100);
		#if _DEBUG
			std::cout << "player bullet hit" << std::endl;
		#endif
	}
}