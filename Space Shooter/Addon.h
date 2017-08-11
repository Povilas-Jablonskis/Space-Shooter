#ifndef _addonH
#define _addonH

#include "BaseGameObject.h"

namespace Engine
{
	class Addon : public BaseGameObject
	{
		public:
			Addon(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, glm::vec2);
		private:
			glm::vec2 positionOffset;
	};
}
#endif