#ifndef collisionManagerH
#define collisionManagerH

#include <vector>

namespace Engine
{
	class CollisionManager
	{
		public:
			template <typename T, typename T2>
			bool checkCollision(const std::shared_ptr<T>& object, const std::shared_ptr<T2>& collider) const
			{
				// Collision x-axis?
				bool collisionX = object->getPosition().x + object->getWidth() >= collider->getPosition().x &&
					collider->getPosition().x + collider->getWidth() >= object->getPosition().x;
				// Collision y-axis?
				bool collisionY = object->getPosition().y + object->getHeight() >= collider->getPosition().y &&
					collider->getPosition().y + collider->getHeight() >= object->getPosition().y;
				// Collision only if on both axes
				return collisionX && collisionY;
			}

			template <typename T, typename T2>
			bool checkCollision(const std::shared_ptr<T>& object, std::vector<std::shared_ptr<T2>>* colliderList)
			{
				if (object->getNeedsToBeRemoved())
				{
					return false;
				}

				for (auto it = colliderList->begin(); it != colliderList->end(); ++it)
				{
					if ((*it)->getNeedsToBeRemoved())
					{
						continue;
					}

					if (checkCollision(object, *it))
					{
						(*it)->onCollisionFunc(object);
						return true;
					}
				}
				return false;
			}
	};
}
#endif