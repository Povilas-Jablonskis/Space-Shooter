#include "Bullet.h"
#include <memory>
#include "Player.h"

namespace Engine
{
	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, std::shared_ptr<BaseGameObject> _parent)
		: BaseGameObject(_width, _height, _position, _velocity, _color), parent(_parent)
	{

	}

	Bullet::~Bullet()
	{

	}

	bool Bullet::update(float _dt)
	{
		if (texture != nullptr)
			BaseGameObject::updateTexture(_dt);

		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (position.y >= windowHeigth || position.y <= 0.0f || position.x >= windowWidth || position.x <= 0.0f)
			return false;
		return true;
	}

	void Bullet::onCollision(BaseGameObject* collider)
	{
		if (parent != nullptr)
		{
			Player* tempPlayer = dynamic_cast<Player*>(parent.get());
			if (tempPlayer != nullptr)
				tempPlayer->setScore(tempPlayer->getScore() + 100);
		}

		std::cout << "bullet hit" << std::endl;
	}

	std::shared_ptr<BaseGameObject> Bullet::getParent()
	{
		if (parent == nullptr) return nullptr;
		return parent;
	}
}