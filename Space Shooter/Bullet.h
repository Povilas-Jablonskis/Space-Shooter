#ifndef _Bullet
#define _Bullet

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			~Bullet();
			Bullet(int, int, float, float, float, float, float, float, float, BaseGameObject*);
			Bullet();
			bool UpdateBullet();
		private:
			BaseGameObject* parent;
	};
}
#endif