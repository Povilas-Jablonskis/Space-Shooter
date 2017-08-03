#ifndef _explosionH
#define _explosionH

#include "RenderObject.h"

namespace Engine
{
	class Explosion : public RenderObject
	{
		public:
			Explosion(int, int, glm::vec2);
			~Explosion();
			bool update(float);
			void applyTexture(std::shared_ptr<Texture>);
		};
}
#endif