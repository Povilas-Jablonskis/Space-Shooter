#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Bullet.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			bool checkCollision(std::shared_ptr<RenderObject>, std::shared_ptr<RenderObject>);
			template <typename T, typename T2>
			void checkCollision(std::shared_ptr<T> object, std::vector<std::shared_ptr<T2>>* colliderList)
			{
				for (std::vector<std::shared_ptr<T2>>::iterator it = colliderList->begin(); it != colliderList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted()) continue;
					if (checkCollision(object, *it))
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

				float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				for (std::vector<std::shared_ptr<Bullet>>::iterator it = bulletList->begin(); it != bulletList->end(); it++)
				{
					if ((*it)->getNeedsToBeDeleted()) continue;
					if ((*it)->getPosition(1) > windowHeigth || ((*it)->getPosition(1) + (*it)->getSize(1)) < 0.0f || (*it)->getPosition(0) > windowWidth || (*it)->getPosition(0) < 0.0f)
					{
 						(*it)->setNeedsToBeDeleted(true);
						continue;
					}
					if (checkCollision(object, *it))
					{
						(*it)->onCollision(object.get(), parent.get());
						return;
					}
				}
			}
	};
}
#endif