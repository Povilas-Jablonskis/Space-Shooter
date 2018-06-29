#include "EnemyColumn.h"

namespace Engine
{
	EnemyColumn::EnemyColumn(glm::vec2 _minPosition, glm::vec2 _maxPosition) : minPosition(_minPosition), maxPosition(_maxPosition)
	{

	}

	bool EnemyColumn::update(float dt)
	{
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			if ((*it)->update(dt))
				it = enemies.erase(it);
			else
				++it;
		}

		if (enemies.size() > 0)
		{
			auto first = enemies.front();
			auto last = enemies.back();

			// Collision x-axis?
			bool collisionX = first->getPosition(0) > minPosition.x &&
				maxPosition.x >= last->getPosition(0) + last->getSize(0);
			// Collision y-axis?
			bool collisionY = last->getPosition(1) > maxPosition.y &&
				minPosition.y >= last->getPosition(1) + last->getSize(1);

			if (!collisionY)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(1, (*it)->getVelocity().y * -1.0f);
				}
			}

			if (!collisionX)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(0, (*it)->getVelocity().x * -1.0f);
				}
			}
		}
		return enemies.size() == 0;
	}
}