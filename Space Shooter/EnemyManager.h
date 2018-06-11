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
			glm::vec2 getRandomMeteorSpawnPoint();
			void generateRandomSpawnPoints();
			void generateRandomMeteorSpawnPoints();
		private:
			glm::vec2 getRandomSpawnPoint();
			std::vector<enemy> enemies;
			std::vector<glm::vec2> spawnPoints;
			std::vector<glm::vec2> meteorSpawnPoints;
	};
}
#endif