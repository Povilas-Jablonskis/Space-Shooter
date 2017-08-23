#ifndef _enemyH
#define _enemyH

#include "BaseGameObject.h"
#include "Bullet.h"

namespace Engine
{
	class Player;
	class Enemy : public BaseGameObject
	{
		public:
			~Enemy();
			Enemy(float, float, glm::vec2, glm::vec2, glm::vec4);
			//bool update(float, float);
			bool update(float);
			void onCollision(Player*);
	};
}
#endif