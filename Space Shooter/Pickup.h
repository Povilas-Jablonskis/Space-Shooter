#ifndef _pickupH
#define _pickupH

#include "Player.h"

namespace Engine
{
	class Pickup : public BaseGameObject
	{
		public:
			Pickup(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			void onCollision(Player*);
			void setEffect(std::function<void(Player*)> _effect) { effect = _effect; }
			std::function<void(Player*)> getEffect() const { return effect; }
		private:
			std::function<void(Player*)> effect;
	};
}
#endif