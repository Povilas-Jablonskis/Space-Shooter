#ifndef _Bullet
#define _Bullet

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			~Bullet();
			Bullet(int, int, glm::vec2, glm::vec2, glm::vec3, BaseGameObject*);
			Bullet();
			bool Update();
		private:
			BaseGameObject* parent;
	};
}
#endif