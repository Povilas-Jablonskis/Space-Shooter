#include "Bullet.h"
#include "Application.h"

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
		updateAnimation(_dt);
		return getNeedsToBeDeleted();
	}

	void Bullet::onCollision(BaseGameObject* collider, Enemy* parent)
	{
		parent->notify(ObserverEvent::BULLETDESTROYED, this);
		setNeedsToBeDeleted(true);

		if (collider != nullptr && !collider->getNeedsToBeDeleted())
			collider->onCollision(this);
	}

	void Bullet::onCollision(BaseGameObject* collider, Player* parent)
	{
		parent->notify(ObserverEvent::BULLETDESTROYED, this);
		setNeedsToBeDeleted(true);

		if (collider != nullptr && !collider->getNeedsToBeDeleted())
			collider->onCollision(this);
	}
}