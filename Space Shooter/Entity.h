#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Addon>> addon;

	class Entity : public BaseGameObject, public Subject
	{
		public:
			Entity(float, float, glm::vec2, glm::vec2, glm::vec4);
			inline std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &bullets; }
			virtual void addBullet(std::shared_ptr<BaseGameObject>);
			inline void clearBullets() { bullets.clear(); }
			inline float getDelayBetweenShootsTimer() const { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			inline std::vector<addon>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			void addAddon(addon);
			void removeAddon(std::string);
			inline void setShootingSound(std::string _shootingSound) { shootingSound = _shootingSound; }
			inline std::string getShootingSound() const { return shootingSound; }
			std::function<void(Entity*)> shootingMode;
		private:
			std::string shootingSound;
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
			std::vector<addon> addons;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
	};
}
#endif