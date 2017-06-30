#include "Bullet.h"
#include <memory>

namespace Engine
{
	Bullet::Bullet()
	{

	}

	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color, BaseGameObject* _parent) 
		: BaseGameObject(_width, _height, _position, _velocity, _color), parent(_parent) {}

	Bullet::~Bullet()
	{

	}

	bool Bullet::Update()
	{
		position[0] += velocity[0];
		position[1] += velocity[1];

		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position[1] + height >= windowheigth || position[1] <= 0.0f || position[0] + width >= windowwidth || position[0] <= 0.0f)
			return false;
		return true;
	}
}