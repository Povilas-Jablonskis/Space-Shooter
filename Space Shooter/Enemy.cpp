#include "Enemy.h"
#include "Player.h"

namespace Engine
{
	//Enemy::Enemy(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
	//	: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShoots(1.5f), delayBetweenShootsTimer(0.0f)
	//{
	//	float _dt = 0.0f;
	//	float maxPos = (float)(glutGet(GLUT_WINDOW_HEIGHT));
	//	float value = 0.0f;
	//	while (_dt < M_PI)
	//	{
	//		value += sin(_dt);
	//		_dt += (1.0f / 60.0f);
	//	}

	//	velocity.y = (maxPos - position.y - height) / value;
	//}

	Enemy::Enemy(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: Entity(_width, _height, _position, _velocity, _color)
	{
		setShootingSound("Sounds/lasers/6.wav");
	}

	void Enemy::addBullet(std::shared_ptr<BaseGameObject> bullet)
	{
		bullet->setVelocity(1, -200.0f);
		bullet->setRotationAngle(3.14159265358979323846f);
		Entity::addBullet(bullet);
	}

	Enemy::~Enemy()
	{
		clearBullets();
	}

	//bool Enemy::update(float dt, float t)
	//{
	//	position.x += velocity.x * dt;
	//	position.y += velocity.y * sin(t);
	//	BaseGameObject::updateAnimation(dt);

	//	delayBetweenShootsTimer += dt;
	//	if (delayBetweenShootsTimer > delayBetweenShoots)
	//	{
	//		delayBetweenShootsTimer = 0.0f;
	//		auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(position.x + (width / 2.0f), position.y - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
	//		bullet->addAnimation("explosion", animations["explosion"]);
	//		bullet->applyAnimation(animations["shoot"]);
	//		bullet->setRotationAngle(3.14159265358979323846f);
	//		bullet->setRotationAxis(glm::vec3(0.0,0.0,1.0));
	//		bullets.push_back(std::move(bullet));
	//	}

	//	for (auto it = bullets.begin(); it != bullets.end();)
	//	{
	//		if ((*it)->update(dt))
	//			it = bullets.erase(it);
	//		else
	//			++it;
	//	}

	//	return getNeedsToBeDeleted();
	//}

	bool Enemy::update(float dt)
	{
		BaseGameObject::update(dt);

		setDelayBetweenShootsTimer(getDelayBetweenShootsTimer() + dt);
		if (getDelayBetweenShootsTimer() > getDelayBetweenShoots())
		{
			setDelayBetweenShootsTimer(0.0f);
			shootingMode(this);
		}

		auto bullets = getBulletsList();
		auto addons = getAddons();

		for (auto it = bullets->begin(); it != bullets->end();)
		{
			if ((*it)->update(dt))
				it = bullets->erase(it);
			else
				++it;
		}

		for (auto it = addons->begin(); it != addons->end();)
		{
			if ((*it).second->update(dt, position))
				it = addons->erase(it);
			else
				++it;
		}

		return getNeedsToBeDeleted();
	}
}