#include "Bullet.h"
#include "Application.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity)
	{

	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateAnimation(_dt);
		return needsToBeDeleted;
	}

	void Bullet::onCollision(Player* collider, Enemy* parent)
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

	void Bullet::onCollision(Enemy* collider, Player* parent)
	{
		if (collider->getAddon("shield") != nullptr)
			collider->removeAddon("shield");
		else
		{
			parent->deleteBullet(this);
			parent->setScore(parent->getScore() + 100);
			collider->setNeedsToBeDeleted(true);
		}
		#if _DEBUG
			std::cout << "player bullet hit" << std::endl;
		#endif
	}
}