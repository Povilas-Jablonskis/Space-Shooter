#include "Bullet.h"
#include <memory>
#include "Player.h"

namespace Engine
{
	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, RenderObject* _parent)
		: RenderObject(_width, _height, _position, _velocity, _color), parent(_parent)
	{

	}

	Bullet::~Bullet()
	{
		delete parent;
	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position.y >= windowHeigth || position.y <= 0.0f || position.x >= windowWidth || position.x <= 0.0f)
			return false;
		return true;
	}

	void Bullet::onCollision(RenderObject* object, RenderObject* collider, CollisionType type)
	{
		std::cout << "bullet hit" << std::endl;
	}
}