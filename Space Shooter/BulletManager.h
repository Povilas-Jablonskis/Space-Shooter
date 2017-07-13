#ifndef _bulletManagerH
#define _bulletManagerH

#include "Bullet.h"

namespace Engine
{
	class BulletManager
	{
		public:
			BulletManager();
			~BulletManager();
			void addBullet(std::shared_ptr<Bullet>);
			void updateBulletList(float);
			void drawBulletList();
			std::vector<std::shared_ptr<Bullet>>* GetBulletList();
			void checkCollision(std::shared_ptr<BaseGameObject>);
			void checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
		private:
			std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif