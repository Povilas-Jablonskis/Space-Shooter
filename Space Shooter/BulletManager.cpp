#include "BulletManager.h"
#include "Renderer.h"

namespace Engine
{
	BulletManager::BulletManager()
	{

	}

	BulletManager::~BulletManager()
	{

	}

	void BulletManager::AddBullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color, BaseGameObject* _parent)
	{
		bullets.push_back(std::make_shared<Bullet>(_width, _height, _position, _velocity, _color, _parent));
	}

	std::vector<std::shared_ptr<BaseGameObject>>& BulletManager::GetBulletsList()
	{
		return bullets;
	}

	void BulletManager::Update()
	{
		for (std::vector<std::shared_ptr<BaseGameObject>> ::iterator it = bullets.begin(); it != bullets.end();)
		{
			if (it->get()->Update() == false)
				it = bullets.erase(it);
			else
				++it;
		}
	}
}