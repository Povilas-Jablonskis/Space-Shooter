#include "BulletManager.h"

namespace Engine
{
	BulletManager::BulletManager()
	{

	}

	BulletManager::~BulletManager()
	{

	}

	void BulletManager::drawBulletList()
	{
		for (auto bullet : bullets)
		{
			bullet->draw();
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

	glm::vec2 BulletManager::checkCollision(std::shared_ptr<BaseGameObject> _objecttocheck)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			auto parent = it->get()->getParent();
			if (parent != nullptr && parent == _objecttocheck) continue;
			auto bullet = it->get();
			if (bullet->checkCollision(_objecttocheck))
			{
				bullets.erase(it);
				return _objecttocheck->getPosition();
			}
		}
		return glm::vec2(0.0f,0.0f);
	}

	glm::vec2 BulletManager::checkCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck)
	{
 		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); ++it)
		{
			for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it2 = _objectstocheck->begin(); it2 != _objectstocheck->end(); ++it2)
			{
				auto parent = it->get()->getParent();
				if (parent != nullptr && parent == *it2) continue;
				auto bullet = it->get();
				if (bullet->checkCollision(*it2))
				{
					bullets.erase(it);
					return (*it2)->getPosition();
				}
			}
		}
		return glm::vec2(0.0f, 0.0f);
	}

	void BulletManager::addBullet(std::shared_ptr<Bullet> bullet, const std::string& texture)
	{
		bullet->applyTexture(texture);
		bullets.push_back(bullet);
	}
}