#ifndef _BulletManager
#define _BulletManager

#include "Bullet.h"

namespace Engine
{
	class BulletManager
	{
		public:
			BulletManager();
			~BulletManager();
			void AddBullet(int, int, glm::vec2, glm::vec2, glm::vec3, BaseGameObject*);
			void Update();
			void Draw();
			std::vector<std::shared_ptr<BaseGameObject>>& GetBulletsList();
		private:
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
	};
}
#endif