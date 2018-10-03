#ifndef _enemyColumnH
#define _enemyColumnH

#include "Entity.h"

namespace Engine
{
	class EnemyColumn
	{
		public:
			EnemyColumn(glm::vec2, glm::vec2);
			bool update(float);
			inline void addEnemy(std::shared_ptr<Entity> _enemy) { enemies.push_back(_enemy); }
			inline std::vector<std::shared_ptr<Entity>>* getEnemies() { return &enemies; }
		private:
			glm::vec2 minPosition;
			glm::vec2 maxPosition;
			std::vector<std::shared_ptr<Entity>> enemies;
	};
}
#endif