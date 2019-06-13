#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "RenderObject.h"
#include "Subject.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class BaseGameObject : public RenderObject, public Subject
	{
		public:
			BaseGameObject(glm::vec2, glm::vec2, glm::vec4);
			std::function<void()> onUpdateFunc;
			virtual bool update(float);
			std::function<void(std::shared_ptr<BaseGameObject>)> onCollisionFunc;
			void addAnimation(std::string, std::shared_ptr<Animation>);
			std::shared_ptr<Animation> getAnimationByIndex(std::string);
			virtual inline void setLives(int _lives) { lives = _lives; }
			inline int getLives() { return lives; }
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline glm::vec2 getVelocity() { return velocity; };
			void setNeedsToBeRemoved(bool boolean) { needsToBeRemoved = boolean; };
			inline bool getNeedsToBeRemoved() { return needsToBeRemoved; }
			inline int getValue() { return value; }
			inline void setValue(int _value) { value = _value; }
			inline std::string getExplosionSound() { return explosionSound; }
			inline void setExplosionSound(std::string _explosionSound) { explosionSound = _explosionSound; }
		private:
			std::string explosionSound;
			bool needsToBeRemoved;
			std::vector<animation> animations;
			glm::vec2 velocity;
			int value;
			int lives;
	};
}
#endif