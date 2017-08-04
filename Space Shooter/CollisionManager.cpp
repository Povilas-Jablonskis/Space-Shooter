#include "CollisionManager.h"

namespace Engine
{
	GLboolean CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::shared_ptr<BaseGameObject> collider) // AABB - AABB collision
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

	std::vector<std::shared_ptr<BaseGameObject>>::iterator CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<BaseGameObject>>* colliderlist) // AABB - AABB collision
	{
		for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = colliderlist->begin(); it != colliderlist->end(); ++it)
		{
			if (checkCollision(*it, object))
				return it;
		}
		return colliderlist->end();
	}

	std::vector<std::shared_ptr<Bullet>>::iterator CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<Bullet>>* bulletlist) // AABB - AABB collision
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bulletlist->begin(); it != bulletlist->end(); ++it)
		{
			if (checkCollision(*it, object))
				return it;
		}
		return bulletlist->end();
	}
}