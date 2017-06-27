#include "Bullet.h"
#include <memory>
#include "Renderer.h"

namespace Engine
{
	Bullet::Bullet()
	{

	}

	Bullet::Bullet(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b, BaseGameObject* _parent) : BaseGameObject(_width, _height, x, y, velx, vely, r, g, b)
	{
		parent = _parent;
	}

	Bullet::~Bullet()
	{

	}

	bool Bullet::UpdateBullet()
	{
		position[0] += velocity[0];
		position[1] += velocity[1];

		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position[1] + height >= windowheigth || position[1] <= 0.0f || position[0] + width >= windowwidth || position[0] <= 0.0f)
			return true;
		return false;
	}
}