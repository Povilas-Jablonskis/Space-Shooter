#ifndef _bulletH
#define _bulletH

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec4, BaseGameObject*);
			bool update(float);
			BaseGameObject* getParent();
			void onCollision(std::shared_ptr<BaseGameObject>);
		private:
			BaseGameObject* parent;
	};
}
#endif