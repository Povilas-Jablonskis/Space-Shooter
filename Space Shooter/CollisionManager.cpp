#include "CollisionManager.h"
#include "Player.h"

namespace Engine
{
	bool CollisionManager::checkCollision(std::shared_ptr<RenderObject> object, std::shared_ptr<RenderObject> collider) // AABB - AABB collision
	{
		// Collision x-axis?
		bool collisionX = object->getPosition(0) + object->getSize(0) >= collider->getPosition(0) && 
			collider->getPosition(0) + collider->getSize(0) >= object->getPosition(0);
		// Collision y-axis?
		bool collisionY = object->getPosition(1) + object->getSize(1) >= collider->getPosition(1) &&
			collider->getPosition(1) + collider->getSize(1) >= object->getPosition(1);
		// Collision only if on both axes
		if (collisionX && collisionY)
			return true;
		return false;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<BaseGameObject>>* colliderList, std::shared_ptr<Entity> parent)
	{
		if (object->getNeedsToBeDeleted() || parent->getNeedsToBeDeleted()) return false;

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		for (auto it = colliderList->begin(); it != colliderList->end(); it++)
		{
			if ((*it)->getNeedsToBeDeleted()) continue;
			if ((*it)->getPosition(1) > windowHeigth || ((*it)->getPosition(1) + (*it)->getSize(1)) < 0.0f || (*it)->getPosition(0) > windowWidth || (*it)->getPosition(0) < 0.0f)
			{
				(*it)->setNeedsToBeDeleted(true);
				continue;
			}
			if (checkCollision(object, *it))
			{
				(*it)->setNeedsToBeDeleted(true);
				parent->notify(ObserverEvent::BULLETDESTROYED, it->get());

				if (object != nullptr && !object->getNeedsToBeDeleted())
				{
					auto player = dynamic_cast<Player*>(parent.get());
					auto entity = dynamic_cast<Entity*>(object.get());
					if (entity != nullptr)
					{
						if (entity->getAddon("shield") != nullptr)
						{
							entity->removeAddon("shield");
							if (player != nullptr)
								player->setScore(player->getScore() + 10);
						}
						else
						{
							entity->setNeedsToBeDeleted(true);
							if (player != nullptr)
								player->setScore(player->getScore() + 100);
						}
					}
					else
					{
						object->setNeedsToBeDeleted(true);
						if (player != nullptr)
							player->setScore(player->getScore() + 100);
					}
				}
				return true;
			}
		}
		return false;
	}
}