#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Bullet.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			GLboolean checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList)
			{
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted()) continue;
					auto collision = checkCollision(object, *it);
					if (collision)
					{
						(*it)->onCollision(object.get());
						return;
					}
				}
			}
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<Bullet>>* bulletList, std::shared_ptr<T2> parent) // AABB - AABB collision
			{
				if (object->getNeedsToBeDeleted() || parent->getNeedsToBeDeleted()) return;
				for (std::vector<std::shared_ptr<Bullet>>::iterator it = bulletList->begin(); it != bulletList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted()) continue;
					auto collision = checkCollision(object, *it);
					if (collision)
					{
						(*it)->onCollision(object.get(), parent.get());
						return;
					}
				}
			}
	};
}
#endif