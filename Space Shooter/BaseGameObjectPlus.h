#ifndef _baseGameObjectPlusH
#define _baseGameObjectPlusH

#include "Entity.h"

namespace Engine
{
	class BaseGameObjectPlus
	{
		public:
			BaseGameObjectPlus(float, float, glm::vec2, glm::vec2, glm::vec4);
			std::function<void(Entity*)> onCollision;
	};
}
#endif