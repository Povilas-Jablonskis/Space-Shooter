#ifndef _playerH
#define _playerH

#include <map>

#include "BaseGameObject.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"

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
			inline float getDelayBetweenShootsTimer() const { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notifyBase(ObserverEvent::HEALTHCHANGED); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notifyBase(ObserverEvent::SCORECHANGED); }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			void deleteBullet(Bullet*);
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
			void onCollision(BaseGameObject*);
			inline std::map<std::string, std::shared_ptr<Addon>>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			inline void addAddon(std::pair<std::string, std::shared_ptr<Addon>> addon) { addons.insert(addon); }
			void removeAddon(std::string);
		private:
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			std::map<std::string, std::shared_ptr<Addon>> addons;
			std::vector<std::shared_ptr<Bullet>> bullets;
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif