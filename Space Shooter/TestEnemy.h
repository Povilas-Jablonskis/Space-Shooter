#ifndef _testEnemyH
#define _testEnemyH

#include "BaseGameObject.h"
#include "Bullet.h"

namespace Engine
{
	class Player;
	class TestEnemy : public BaseGameObject
	{
		public:
			~TestEnemy();
			TestEnemy(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, float);
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			void deleteBullet(Bullet*);
			void onCollision(Player*);
		private:
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif