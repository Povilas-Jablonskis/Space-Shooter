#include "Bullet.h"
#include <memory>
#include "Player.h"

namespace Engine
{
	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, BaseGameObject* _parent) 
		: BaseGameObject(_width, _height, _position, _velocity, _color), parent(_parent)
	{
	
	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position.y + height >= windowHeigth || position.y <= 0.0f || position.x + width >= windowWidth || position.x <= 0.0f)
			return false;
		return true;
	}

	void Bullet::onCollision(BaseGameObject* collider)
	{
		if (parent != nullptr)
		{
			Player* tempPlayer = dynamic_cast<Player*>(parent);
			if (tempPlayer != nullptr)
				tempPlayer->setScore(tempPlayer->getScore() + 100);
		}

		std::cout << "bullet hit" << std::endl;
	}

	BaseGameObject* Bullet::getParent()
	{
		return parent;
	}
}