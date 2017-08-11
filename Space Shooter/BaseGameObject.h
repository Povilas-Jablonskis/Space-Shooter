#ifndef _baseGameObjectH
#define _baseGameObjectH

#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <memory>
#include <map>

#include "RenderObject.h"

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
			inline void setNeedsToBeDeleted(bool boolean) { needsToBeDeleted = boolean; };
			inline bool getNeedsToBeDeleted() const { return needsToBeDeleted; }
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			virtual void onCollision(BaseGameObject*);
			std::function<void()> onDeath;
		protected:
			std::map<std::string, std::shared_ptr<Animation>> animations;
			bool needsToBeDeleted;
			glm::vec2 velocity;
	};
}
#endif