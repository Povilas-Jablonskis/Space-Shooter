#ifndef _pickupH
#define _pickupH

#include "BaseGameObject.h"

namespace Engine
{
	class Pickup : public BaseGameObject
	{
		public:
			Pickup(float, float, glm::vec2, glm::vec2, glm::vec4);
			std::function<bool(std::shared_ptr<BaseGameObject>)> effect;
	};
}
#endif