#include "CollisionManager.h"

namespace Engine
{
	bool CollisionManager::checkCollision(std::shared_ptr<RenderObject> object, std::shared_ptr<RenderObject> collider) // AABB - AABB collision
	{
		// Collision x-axis?
		bool collisionX = object->getPosition().x + object->getWidth() >= collider->getPosition().x && 
			collider->getPosition().x + collider->getWidth() >= object->getPosition().x;
		// Collision y-axis?
		bool collisionY = object->getPosition().y + object->getHeight() >= collider->getPosition().y &&
			collider->getPosition().y + collider->getHeight() >= object->getPosition().y;
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
			if ((*it)->getPosition().y > windowHeigth || ((*it)->getPosition().y + (*it)->getHeight()) < 0.0f || (*it)->getPosition().x > windowWidth || (*it)->getPosition().x < 0.0f)
			{
				it = bulletList->erase(it);
				continue;
			}
			if (checkCollision(object, *it))
			{
				(*it)->onCollisionFunc(object);
				return true;
			}
			++it;
		}
		return false;
	}
}