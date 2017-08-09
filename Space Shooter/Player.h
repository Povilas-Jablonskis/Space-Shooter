#ifndef _playerH
#define _playerH

#include <map>

#include "BaseGameObject.h"
#include "InputManager.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			~Player();
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
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
			int lastScore;
			int score;
			int startHealth;
			int lastHealth;
			int health;
	};
}
#endif