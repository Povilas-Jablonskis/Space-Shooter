#ifndef _playerH
#define _playerH

#include <map>

#include "BaseGameObject.h"
#include "InputManager.h"
#include "Bullet.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			Player(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Player();
			bool update(float, std::shared_ptr<InputManager>);
			void respawn();
			void restart();
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { lastHealth = health; health = _health; }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { lastScore = score; score = _score; }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			void deleteBullet(Bullet*);
			inline int getLastScore() const { return lastScore; }
			inline int getLastHealth() const { return lastHealth; }
			void onCollision(BaseGameObject*);
		private:
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			std::vector<std::shared_ptr<Bullet>> bullets;
			glm::vec2 startVelocity;
			int lastScore;
			int score;
			int startHealth;
			int lastHealth;
			int health;
	};
}
#endif