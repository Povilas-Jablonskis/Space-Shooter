#ifndef _addonH
#define _addonH

#include "RenderObject.h"

namespace Engine
{
	class Addon : public RenderObject
	{
		public:
			Addon(float, float, glm::vec2);
			bool update(float, glm::vec2);
		private:
			glm::vec2 positionOffset;
	};
}
#endif