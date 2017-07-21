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
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
			inline void clearBullets() { bullets.clear(); }
			void updateBulletList(float);
			void drawBulletList(GLuint, GLuint);
			void checkCollision(std::shared_ptr<BaseGameObject>);
			void checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
		private:
			std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif