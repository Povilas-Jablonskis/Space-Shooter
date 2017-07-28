#ifndef _explosionH
#define _explosionH

#include "BaseGameObject.h"
#include "Application.h"

namespace Engine
{
	class Explosion : public BaseGameObject
	{
		public:
			Explosion(int, int, glm::vec2, glm::vec2, glm::vec4, std::shared_ptr<Application>);
			~Explosion();
			void updateTexture(float);
		};
}
#endif