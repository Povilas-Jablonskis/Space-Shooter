#include "Bullet.h"
#include "Application.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		addObserver(ObserverEvent::COLLISIONHAPPEND, [this]()
		{ 
			auto explosion = std::make_shared<Explosion>(32.0f, 32.0f, getPosition());
			explosion->applyAnimation(getAnimationByIndex("explosion"));
			Application::instance()->addExplosionToList(std::move(explosion));
		});
	}

	void Bullet::onCollision(Player* collider, TestEnemy* parent)
	{
		parent->deleteBullet(this);
		if (collider->getAddon("shield") != nullptr)
			collider->removeAddon("shield");
		else
		{
			notify(ObserverEvent::COLLISIONHAPPEND);
			collider->respawn();
		}
		#if _DEBUG
			std::cout << "enemy bullet hit" << std::endl;
		#endif
	}

	void Bullet::onCollision(TestEnemy* collider, Player* parent)
	{
		collider->setNeedsToBeDeleted(true);
		parent->deleteBullet(this);
		parent->setScore(parent->getScore() + 100);
		notify(ObserverEvent::COLLISIONHAPPEND);
		#if _DEBUG
			std::cout << "player bullet hit" << std::endl;
		#endif
	}
}