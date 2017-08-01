#include "Bullet.h"
#include <memory>
#include "Player.h"

namespace Engine
{
	Bullet::Bullet(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, BaseGameObject* _parent)
		: BaseGameObject(_width, _height, _position, _velocity, _color), parent(_parent)
	{

	}

	Bullet::~Bullet()
	{

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

	void Bullet::updateTexture(float dt)
	{
		if (texture == nullptr) return;
		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			animTimer += dt;
			if (animTimer > delay)
			{
				animTimer -= delay;
				currentFrame++;
				if (currentFrame < texture->getStartFrame() || currentFrame > texture->getEndFrame())
				{
					if (loop == true)
						currentFrame = texture->getStartFrame();
					else
					{
						currentFrame = texture->getEndFrame();
						animComplete = true;
					}
				}
			}
		}
	}

	void Bullet::onCollision(BaseGameObject* collider)
	{
		std::cout << "bullet hit" << std::endl;
	}
}