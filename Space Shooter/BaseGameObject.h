#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>
#include <map>

#include "RenderObject.h"
#include "ShootingType.h"
#include "Bullet.h"
#include "Addon.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class BaseGameObject : public RenderObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(float, float, glm::vec2, glm::vec2, glm::vec4);
			virtual bool update(float);
			void addAnimation(std::string, std::shared_ptr<Animation>);
			std::shared_ptr<Animation> getAnimationByIndex(std::string index) { return animations[index]; }
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			virtual void onCollision(BaseGameObject*);
			std::function<void()> onDeath;
			inline float getDelayBetweenShootsTimer() const { return delayBetweenShootsTimer; }
			inline void setDelayBetweenShootsTimer(float _delayBetweenShootsTimer) { delayBetweenShootsTimer = _delayBetweenShootsTimer; }
			inline float getDelayBetweenShoots() const { return delayBetweenShoots; }
			inline void setDelayBetweenShoots(float _delayBetweenShoots) { delayBetweenShoots = _delayBetweenShoots; }
			ShootingType getShootingType() const { return shootingType; }
			void setShootingType(ShootingType _shootingType) { shootingType = _shootingType; }
			inline std::vector<std::shared_ptr<Bullet>>* getBulletsList() { return &bullets; }
			void deleteBullet(Bullet*);
			inline void addBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }
			inline std::map<std::string, std::shared_ptr<Addon>>* getAddons() { return &addons; }
			std::shared_ptr<Addon> getAddon(std::string);
			inline void addAddon(std::pair<std::string, std::shared_ptr<Addon>> addon) { addons.insert(addon); }
			void removeAddon(std::string);
		protected:
			std::map<std::string, std::shared_ptr<Addon>> addons;
			float delayBetweenShootsTimer;
			float delayBetweenShoots;
			std::vector<std::shared_ptr<Bullet>> bullets;
			ShootingType shootingType;
			std::map<std::string, std::shared_ptr<Animation>> animations;
			glm::vec2 velocity;
	};
}
#endif