#include "BulletManager.h"

namespace Engine
{
	std::vector<std::shared_ptr<Bullet>> BulletManager::bullets;

	void BulletManager::AddBullet(std::shared_ptr<Bullet> bullet)
	{
		bullets.push_back(bullet);
	}

	std::vector<std::shared_ptr<Bullet>>* BulletManager::GetBulletList()
	{
		return &bullets;
	}
}