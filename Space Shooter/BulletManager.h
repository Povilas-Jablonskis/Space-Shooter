#ifndef _BulletManager
#define _BulletManager

#include "Bullet.h"

namespace Engine
{
	class BulletManager
	{
		public:
			static void AddBullet(std::shared_ptr<Bullet>);
			static std::vector<std::shared_ptr<Bullet>>* GetBulletList();
		private:
			static std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif