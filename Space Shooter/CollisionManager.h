#ifndef _collisionManagerH
#define _collisionManagerH

#include "BaseGameObject.h"

namespace Engine
{
	class CollisionManager
	{
		public:
			CollisionManager();
			~CollisionManager();
			GLboolean checkCollision(std::shared_ptr<BaseGameObject>, std::shared_ptr<BaseGameObject>);
			std::vector<std::shared_ptr<BaseGameObject>>::iterator checkCollision(std::shared_ptr<BaseGameObject>, std::vector<std::shared_ptr<BaseGameObject>>*);
	};
}
#endif