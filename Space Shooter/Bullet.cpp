#include "Bullet.h"
#include <memory>

namespace Engine
{
	Bullet::Bullet()
	{

	}

	Bullet::Bullet(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b) : BaseGameObject(_width, _height, x, y, velx, vely, r, g, b)
	{
	}

	Bullet::~Bullet()
	{

	}

	std::vector<std::shared_ptr<BaseGameObject>>::iterator Bullet::Update(std::vector<std::shared_ptr<BaseGameObject>>& vector, std::vector<std::shared_ptr<BaseGameObject>>::iterator _it)
	{
		position[0] += velocity[0];
		position[1] += velocity[1];

		return BaseGameObject::Update(vector, _it);
	}
}