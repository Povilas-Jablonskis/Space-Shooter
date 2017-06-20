#include "BulletManager.h"

namespace Engine
{
	std::vector<BaseGameObject*> BulletManager::bullets;

	void BulletManager::AddBullet(Bullet* bullet)
	{
		bullets.push_back(bullet);
	}

	std::vector<BaseGameObject*> BulletManager::GetBulletList()
	{
		return bullets;
	}
}