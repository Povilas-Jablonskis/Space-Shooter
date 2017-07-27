#ifndef _bulletH
#define _bulletH

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec4, std::shared_ptr<BaseGameObject>, std::shared_ptr<Application>);
			~Bullet();
			bool update(float);
			std::shared_ptr<BaseGameObject> getParent();
			void onCollision(BaseGameObject*);
		private:
			std::shared_ptr<BaseGameObject> parent;
	};
}
#endif