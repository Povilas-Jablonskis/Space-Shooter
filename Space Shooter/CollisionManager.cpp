#include "CollisionManager.h"

namespace Engine
{
	GLboolean CollisionManager::checkCollision(std::shared_ptr<RenderObject> object, std::shared_ptr<RenderObject> collider) // AABB - AABB collision
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
}