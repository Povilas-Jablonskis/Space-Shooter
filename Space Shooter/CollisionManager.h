#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"
#include "Bullet.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			CollisionManager();
			~CollisionManager();
			std::vector<std::shared_ptr<BaseGameObject>>::iterator checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<BaseGameObject>>*);
			std::vector<std::shared_ptr<Bullet>>::iterator checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<Bullet>>*);
		private:
			GLboolean checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
	};
}
#endif