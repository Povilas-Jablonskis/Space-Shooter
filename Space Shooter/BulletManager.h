#ifndef _bulletManagerH
#define _bulletManagerH

#include "Bullet.h"
#include "Renderer.h"

namespace Engine
{
	class BulletManager
	{
		public:
			BulletManager();
			~BulletManager();
			void addBullet(std::shared_ptr<Bullet>, std::shared_ptr<Texture> = nullptr);
			inline void clearBullets() { bullets.clear(); }
			void updateBulletList(float);
			void drawBulletList(std::shared_ptr<Renderer>);
			glm::vec2 checkCollision(std::shared_ptr<BaseGameObject>);
			glm::vec2 checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
		private:
			std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif