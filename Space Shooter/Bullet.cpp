#include "Bullet.h"
#include "Application.h"

namespace Engine
{
	Bullet::Bullet(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{

	}

	bool Bullet::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);

		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		 
		if (needsToBeDeleted || position.y > windowHeigth || (position.y + height) < 0.0f || position.x > windowWidth || position.x < 0.0f)
			return true;
		return needsToBeDeleted;
	}

	void Bullet::onCollision(Player* collider, TestEnemy* parent)
	{
		parent->deleteBullet(this);
		if (collider->getAddon("shield") != nullptr)
			collider->removeAddon("shield");
		else
		{
			auto explosion = std::make_shared<Explosion>(32.0f, 32.0f, collider->getPosition());
			explosion->applyTexture(parent->getAnimation("explosion"));
			Application::instance()->addExplosionToList(std::move(explosion));
			collider->respawn();
		}
		#if _DEBUG
			std::cout << "enemy bullet hit" << std::endl;
		#endif
	}

	void Bullet::onCollision(TestEnemy* collider, Player* parent)
	{
		collider->setNeedsToBeDeleted(true);
		parent->deleteBullet(this);
		parent->setScore(parent->getScore() + 100);
		auto explosion = std::make_shared<Explosion>(32.0f, 32.0f, collider->getPosition());
		explosion->applyTexture(parent->getAnimation("explosion"));
		Application::instance()->addExplosionToList(std::move(explosion));
		#if _DEBUG
			std::cout << "player bullet hit" << std::endl;
		#endif
	}
}