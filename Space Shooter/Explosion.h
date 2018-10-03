#ifndef _explosionH
#define _explosionH

#include "RenderObject.h"

namespace Engine
{
	class Explosion : public RenderObject
	{
		public:
			Explosion(float, float, glm::vec2);
			bool update(float);
		};
}
#endif