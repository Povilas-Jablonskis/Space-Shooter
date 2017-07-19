#include "BulletManager.h"

namespace Engine
{
	BulletManager::BulletManager()
	{

	}

	BulletManager::~BulletManager()
	{

	}

	void BulletManager::drawBulletList(GLuint program, GLuint vao)
	{
		for (auto bullet : bullets)
		{
			bullet->draw(program, vao);
		}
	}

	void BulletManager::updateBulletList(float dt)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			auto bullet = it->get();
			if (bullet->update(dt) == false)
				it = bullets.erase(it);
			else
				it++;
		}
	}

	void BulletManager::checkCollision(std::shared_ptr<BaseGameObject> _objecttocheck)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			auto parent = it->get()->getParent();
			if (parent != nullptr && parent == _objecttocheck.get()) continue;
			auto bullet = it->get();
			if (bullet->checkCollision(_objecttocheck))
			{
				bullets.erase(it);
				return;
			}
		}
	}

	void BulletManager::checkCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck)
	{
 		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it2 = _objectstocheck->begin(); it2 != _objectstocheck->end(); ++it2)
			{
				auto parent = it->get()->getParent();
				if (parent != nullptr && parent == it2->get()) continue;
				auto bullet = it->get();
				if (bullet->checkCollision(*it2))
				{
					bullets.erase(it);
					_objectstocheck->erase(it2);
					return;
				}
			}
		}
	}
}