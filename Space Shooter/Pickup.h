#ifndef _pickupH
#define _pickupH

#include "Player.h"

namespace Engine
{
	class Pickup : public RenderObject
	{
		public:
			Pickup(float, float, glm::vec2);
			bool update(float);
			void onCollision(Player*);
			void setEffect(std::function<void(BaseGameObject*)> _effect) { effect = _effect; }
			std::function<void(BaseGameObject*)> getEffect() const { return effect; }
		private:
			std::function<void(BaseGameObject*)> effect;
	};
}
#endif