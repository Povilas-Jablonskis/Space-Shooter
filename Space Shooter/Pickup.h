#ifndef _pickupH
#define _pickupH

#include "Player.h"

namespace Engine
{
	class Pickup : public BaseGameObject
	{
		public:
			Pickup(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, glm::vec2);
			std::function<void(Player*)> onCollision;
			bool update(float);
	};
}
#endif