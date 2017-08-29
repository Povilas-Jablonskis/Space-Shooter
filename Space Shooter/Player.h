#ifndef _playerH
#define _playerH

#include "BaseGameObject.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"
#include "ShootingType.h"

namespace Engine
{
	class Player : public BaseGameObject, public Subject
	{
		public:
			~Player();
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			void respawn();
			void restart();
			void onCollision(BaseGameObject*);
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notify(ObserverEvent::HEALTHCHANGED, this); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notify(ObserverEvent::SCORECHANGED, this); }
			inline float getDelayBetweenShootsTimer() const { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			inline std::vector<std::pair<std::string, std::shared_ptr<Addon>>>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			void addAddon(std::pair<std::string, std::shared_ptr<Addon>>);
			void removeAddon(std::string);
			ShootingType getShootingType() const { return shootingType; }
			void setShootingType(ShootingType _shootingType) { shootingType = _shootingType; }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
		private:
			ShootingType shootingType;
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::pair<std::string, std::shared_ptr<Addon>>> addons;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif