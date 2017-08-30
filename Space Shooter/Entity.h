#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"
#include "ShootingType.h"

namespace Engine
{
	class Entity : public BaseGameObject, public Subject
	{
		public:
			Entity(float, float, glm::vec2, glm::vec2, glm::vec4);
			ShootingType getShootingType() const { return shootingType; }
			void setShootingType(ShootingType _shootingType) { shootingType = _shootingType; }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
			inline void clearBullets() { bullets.clear(); }
			inline float getDelayBetweenShootsTimer() const { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			inline std::vector<std::pair<std::string, std::shared_ptr<Addon>>>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			void addAddon(std::pair<std::string, std::shared_ptr<Addon>>);
			void removeAddon(std::string);
		private:
			ShootingType shootingType;
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::pair<std::string, std::shared_ptr<Addon>>> addons;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
	};
}
#endif