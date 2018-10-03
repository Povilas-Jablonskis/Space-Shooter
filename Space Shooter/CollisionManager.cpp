#include "CollisionManager.h"

namespace Engine
{
	bool CollisionManager::checkCollision(std::shared_ptr<RenderObject> object, std::shared_ptr<RenderObject> collider) // AABB - AABB collision
	{
		// Collision x-axis?
		bool collisionX = object->getPosition(0) + object->getWidth() >= collider->getPosition(0) && 
			collider->getPosition(0) + collider->getWidth() >= object->getPosition(0);
		// Collision y-axis?
		bool collisionY = object->getPosition(1) + object->getHeight() >= collider->getPosition(1) &&
			collider->getPosition(1) + collider->getHeight() >= object->getPosition(1);
		// Collision only if on both axes
		if (collisionX && collisionY)
			return true;
		return false;
	}

	bool CollisionManager::checkCollision(std::shared_ptr<BaseGameObject> object, std::vector<std::shared_ptr<BaseGameObject>>* bulletList, std::shared_ptr<Entity> parent)
	{
		if (object->getNeedsToBeRemoved() || parent->getNeedsToBeRemoved()) return false;

		auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		for (auto it = bulletList->begin(); it != bulletList->end();)
		{
			if ((*it)->getNeedsToBeRemoved())
			{
				++it;
				continue;
			}
			if ((*it)->getPosition(1) > windowHeigth || ((*it)->getPosition(1) + (*it)->getHeight()) < 0.0f || (*it)->getPosition(0) > windowWidth || (*it)->getPosition(0) < 0.0f)
			{
				it = bulletList->erase(it);
				continue;
			}
			if (checkCollision(object, *it))
			{
				(*it)->onCollision(object);
				return true;
			}
			++it;
		}
		return false;
	}
}