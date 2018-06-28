#ifndef _enemyColumnH
#define _enemyColumnH

#include "Enemy.h"

namespace Engine
{
	class EnemyColumn
	{
		public:
			EnemyColumn(glm::vec2);
			bool update(float);
			inline void addEnemy(std::shared_ptr<Enemy> _enemy) { enemies.push_back(_enemy); }
			inline std::vector<std::shared_ptr<Enemy>>* getEnemies() { return &enemies; }
		private:
			glm::vec2 maxPosition;
			std::vector<std::shared_ptr<Enemy>> enemies;
	};
}
#endif