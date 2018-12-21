#ifndef _pickupH
#define _pickupH

#include "BaseGameObject.h"

namespace Engine
{
	class Pickup : public RenderObject, public Subject
	{
		public:
			~Pickup();
			Pickup(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			std::function<void(std::shared_ptr<BaseGameObject>)> onCollision;
			void addAnimation(std::string, std::shared_ptr<Animation>);
			std::shared_ptr<Animation> getAnimationByIndex(std::string);
			inline void setVelocity(glm::vec2 _velocity) { velocity = _velocity; };
			inline void setVelocity(int index, float _velocity) { velocity[index] = _velocity; };
			inline float getVelocity(int index) { return velocity[index]; };
			inline glm::vec2 getVelocity() { return velocity; };
			void setNeedsToBeRemoved(bool boolean) { needsToBeRemoved = boolean; };
			inline bool getNeedsToBeRemoved() { return needsToBeRemoved; }
			void applyAnimation(std::shared_ptr<Animation>);
		private:
			bool needsToBeRemoved;
			std::vector<animation> animations;
			glm::vec2 velocity;
	};
}
#endif