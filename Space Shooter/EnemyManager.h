#ifndef _enemyManagerH
#define _enemyManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <vec2.hpp>

#include "EffectManager.h"
#include "Enemy.h"
#include "SpriteSheetManager.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Enemy>> enemy;

	class EnemyManager
	{
		public:
			void loadEnemiesFromConfig(std::shared_ptr<SpriteSheetManager>, std::shared_ptr<EffectManager>);
			std::shared_ptr<Enemy> getEnemy(std::string);
			std::shared_ptr<Enemy> getRandomEnemy();
		private:
			std::vector<enemy> enemies;
	};
}
#endif