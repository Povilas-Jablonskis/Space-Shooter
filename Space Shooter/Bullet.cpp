#include "Bullet.h"
#include "Application.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}

	void Bullet::onCollision(Entity* collider, Entity* parent)
	{
		parent->notify(ObserverEvent::BULLETDESTROYED, this);
		setNeedsToBeDeleted(true);

		if (collider != nullptr && !collider->getNeedsToBeDeleted())
		{
			auto player = dynamic_cast<Player*>(parent);
			if (collider->getAddon("shield") != nullptr)
			{
				collider->removeAddon("shield");
				if (player != nullptr)
					player->setScore(player->getScore() + 10);
			}
			else
			{
				collider->setNeedsToBeDeleted(true);
				if (player != nullptr)
					player->setScore(player->getScore() + 100);
			}
		}
	}

	void Bullet::onCollision(BaseGameObject* collider, Entity* parent)
	{
		parent->notify(ObserverEvent::BULLETDESTROYED, this);
		setNeedsToBeDeleted(true);

		if (collider != nullptr && !collider->getNeedsToBeDeleted())
		{
			auto player = dynamic_cast<Player*>(parent);
			if (player != nullptr)
				player->setScore(player->getScore() +  25);

			collider->setNeedsToBeDeleted(true);
		}
	}
}