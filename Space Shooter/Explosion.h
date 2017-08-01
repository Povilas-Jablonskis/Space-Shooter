#ifndef _explosionH
#define _explosionH

#include "BaseGameObject.h"

namespace Engine
{
	class Explosion : public BaseGameObject
	{
		public:
			Explosion(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Explosion();
			bool update(float);
			void applyTexture(std::shared_ptr<Texture>);
		};
}
#endif