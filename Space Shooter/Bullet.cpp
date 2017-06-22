#include "Bullet.h"

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

	void Bullet::Update()
	{
		position[0] += velocity[0];
		position[1] += velocity[1];
	}
}