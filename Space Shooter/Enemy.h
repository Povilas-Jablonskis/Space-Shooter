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
			void addBullet(std::shared_ptr<BaseGameObject>);
			bool update(float);
	};
}
#endif