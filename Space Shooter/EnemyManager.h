#ifndef _enemyManagerH
#define _enemyManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <vec2.hpp>

#include "PickupManager.h"
#include "Entity.h"
#include "SpriteSheetManager.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Entity>> enemy;

	class EnemyManager
	{
		public:
			void loadEnemiesFromConfig(std::shared_ptr<SpriteSheetManager>, std::shared_ptr<PickupManager>);
			std::shared_ptr<Entity> getEnemy(std::string);
			std::shared_ptr<Entity> getRandomEnemy();
		private:
			std::vector<enemy> enemies;
	};
}
#endif