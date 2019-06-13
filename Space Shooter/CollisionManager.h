#ifndef _collisionManagerH
#define _collisionManagerH

#include "Entity.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			bool checkCollision(std::shared_ptr<RenderObject>, std::shared_ptr<RenderObject>);
			bool checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<BaseGameObject>>*, std::shared_ptr<Entity>);
			template <typename T, typename T2>
			bool checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList)
			{
				for (auto it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeRemoved()) continue;
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