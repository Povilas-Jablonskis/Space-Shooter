#ifndef _BulletManager
#define _BulletManager

#include "Bullet.h"

namespace Engine
{
	class BulletManager
	{
		public:
			static void AddBullet(Bullet*);
			static std::vector<BaseGameObject*> GetBulletList();
		private:
			static std::vector<BaseGameObject*> bullets;
	};
}
#endif