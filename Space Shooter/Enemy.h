#ifndef _enemyH
#define _enemyH

#include "Entity.h"

namespace Engine
{
	class Enemy : public Entity
	{
		public:
			~Enemy();
			Enemy(float, float, glm::vec2, glm::vec2, glm::vec4);
			//bool update(float, float);
			void onCollision(Entity*);
			bool update(float);
	};
}
#endif