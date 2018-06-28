#include "EnemyColumn.h"

namespace Engine
{
	EnemyColumn::EnemyColumn(glm::vec2 _maxPosition) : maxPosition(_maxPosition)
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
			float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
			float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
			auto first = enemies.front();
			auto last = enemies.back();

			if (last->getPosition(0) + last->getSize(0) >= windowWidth)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(0, (*it)->getVelocity().x * -1.0f);
				}
			}
			else if (last->getPosition(0) + last->getSize(0) >= maxPosition.x)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(0, (*it)->getVelocity().x * -1.0f);
				}
			}
			else if (first->getPosition(0) <= 0.0f)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(0, (*it)->getVelocity().x * -1.0f);
				}
			}

			if (first->getPosition(1) + first->getSize(1) >= windowHeigth)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(1, (*it)->getVelocity().y * -1.0f);
				}
			}
			else if (first->getPosition(1) + first->getSize(1) <= maxPosition.y)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(1, (*it)->getVelocity().y * -1.0f);
				}
			}
			else if (first->getPosition(1) <= 0.0f)
			{
				for (auto it = enemies.begin(); it != enemies.end(); it++)
				{
					(*it)->setVelocity(1, (*it)->getVelocity().y * -1.0f);
				}
			}
		}
		return enemies.size() == 0;
	}
}