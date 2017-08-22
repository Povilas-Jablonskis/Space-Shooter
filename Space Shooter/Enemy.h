#ifndef _enemyH
#define _enemyH

#include "BaseGameObject.h"
#include "Bullet.h"
#include "ShootingType.h"

namespace Engine
{
	class Player;
	class Enemy : public BaseGameObject
	{
		public:
			~Enemy();
			Enemy(float, float, glm::vec2, glm::vec2, glm::vec4);
			//bool update(float, float);
			bool update(float);
			ShootingType getShootingType() const { return shootingType; }
			void setShootingType(ShootingType _shootingType) { shootingType = _shootingType; }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			void deleteBullet(Bullet*);
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
			void onCollision(Player*);
		private:
			ShootingType shootingType;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			std::vector<std::shared_ptr<Bullet>> bullets;
	};
}
#endif