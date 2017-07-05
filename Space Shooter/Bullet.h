#ifndef _bulletH
#define _bulletH

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			~Bullet();
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec3, BaseGameObject*);
			Bullet();
			bool Update(float);
			GLboolean CheckCollision(std::shared_ptr<BaseGameObject>);
			void OnCollision(std::shared_ptr<BaseGameObject> collider);
		private:
			BaseGameObject* parent;
	};
}
#endif