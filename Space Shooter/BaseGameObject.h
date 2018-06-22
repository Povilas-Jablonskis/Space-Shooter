#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>

#include "RenderObject.h"
#include "Addon.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"
#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Entity;
	class BaseGameObject : public RenderObject
	{
		public:
			~BaseGameObject();
			BaseGameObject(float, float, glm::vec2, glm::vec2, glm::vec4);
			virtual bool update(float);
			std::function<void(std::shared_ptr<BaseGameObject>)> onCollision;
			void addAnimation(std::string, std::shared_ptr<Animation>);
			std::shared_ptr<Animation> getAnimationByIndex(std::string);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			std::function<void()> onDeath;
			inline void setNeedsToBeRemoved(bool boolean) { needsToBeRemoved = boolean; };
			inline bool getNeedsToBeRemoved() const { return needsToBeRemoved; }
		protected:
			bool needsToBeRemoved;
			std::vector<animation> animations;
			glm::vec2 velocity;
	};
}
#endif