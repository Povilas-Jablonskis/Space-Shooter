#ifndef _entityH
#define _entityH

#include "BaseGameObject.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<BaseGameObject>> addon;

	class Application;
	class Entity : public BaseGameObject
	{
		public:
			Entity(float, float, glm::vec2, glm::vec2, glm::vec4);
			inline std::vector<std::shared_ptr<BaseGameObject>>* getBulletsList() { return &bullets; }
			void addBullet(std::shared_ptr<BaseGameObject>);
			virtual bool update(float);
			inline void clearBullets() { bullets.clear(); }
			inline float getDelayBetweenShootsTimer() { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			inline std::vector<addon>* getAddons() { return &addons; }
			std::shared_ptr<BaseGameObject> getAddon(std::string);
			void addAddon(addon);
			std::function<void()> shootingMode;
		private:
			std::vector<std::shared_ptr<BaseGameObject>> bullets;
			std::vector<addon> addons;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
	};
}
#endif