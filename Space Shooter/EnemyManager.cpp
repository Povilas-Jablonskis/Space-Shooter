#include "EnemyManager.h"
#include <fstream>

namespace Engine
{
	void EnemyManager::loadEnemiesFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, std::shared_ptr<EffectManager> effectManager)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/enemies.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Enemies");
		//Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Enemy"); brewery_node; brewery_node = brewery_node->next_sibling("Enemy"))
		{
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			std::string shootingMode = brewery_node->first_attribute("shootingMode")->value();
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto _enemy = std::make_shared<Enemy>(32.0f, 32.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 160.0f, 122.0f, 1.0f));
			_enemy->applyAnimation(sprite);
			effectManager->getEffect(shootingMode)(_enemy);

			for (auto beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
			{
				for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
				{
					_enemy->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));
				}
				for (auto beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
				{
					_enemy->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node2->first_attribute("animationName")->value()));
				}
			}

			enemies.push_back(enemy(brewery_node->first_attribute("name")->value(), std::move(_enemy)));
		}
	}

	std::shared_ptr<Enemy> EnemyManager::getEnemy(std::string index)
	{
		for (auto enemy : enemies)
		{
			if (enemy.first == index)
			{
				auto _enemy = std::make_shared<Enemy>(*enemy.second);
				_enemy->setPosition(getRandomSpawnPoint());
				return _enemy;
			}
		}

		return nullptr;
	}

	std::shared_ptr<Enemy> EnemyManager::getRandomEnemy()
	{
		int randIndex = rand() % enemies.size();
		auto _enemy = std::make_shared<Enemy>(*enemies[randIndex].second);
		_enemy->setPosition(getRandomSpawnPoint());
		return _enemy;
	}

	glm::vec2 EnemyManager::getRandomSpawnPoint()
	{
		int rnd = rand() % spawnPoints.size();
		auto spawnPoint = spawnPoints[rnd];
		spawnPoints.erase(spawnPoints.begin() + rnd);
		return spawnPoint;
	}

	glm::vec2 EnemyManager::getRandomMeteorSpawnPoint()
	{
		int rnd = rand() % meteorSpawnPoints.size();
		auto spawnPoint = meteorSpawnPoints[rnd];
		meteorSpawnPoints.erase(meteorSpawnPoints.begin() + rnd);
		return spawnPoint;
	}

	void EnemyManager::generateRandomSpawnPoints()
	{
		spawnPoints.clear();
		for (float x = 0.0f; x < ((float)glutGet(GLUT_WINDOW_WIDTH) - 32.0f);)
		{
			for (float y = 340.0f; y < ((float)glutGet(GLUT_WINDOW_HEIGHT) - 32.0f);)
			{
				spawnPoints.push_back(glm::vec2(x, y));
				y += 64.0f;
			}
			x += 96.0f;
		}
	}

	void EnemyManager::generateRandomMeteorSpawnPoints()
	{
		meteorSpawnPoints.clear();
		for (float x = 0.0f; x < ((float)glutGet(GLUT_WINDOW_WIDTH) - 32.0f);)
		{
			for (float y = 100.0f; y < 300.0f;)
			{
				meteorSpawnPoints.push_back(glm::vec2(x, y));
				y += 32.0f;
			}
			x += 32.0f;
		}
	}
}