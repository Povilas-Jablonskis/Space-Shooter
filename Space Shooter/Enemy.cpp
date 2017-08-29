#include "Enemy.h"
#include "Player.h"

#define _USE_MATH_DEFINES
#include <math.h>

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
		: BaseGameObject(_width, _height, _position, _velocity, _color), delayBetweenShoots(0.2f), delayBetweenShootsTimer(0.0f), shootingType(ShootingType::NONE)
	{

	}

	Enemy::~Enemy()
	{
		bullets.clear();
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
	//		bullet->setRotationAngle((float)M_PI);
	//		bullet->setRotationAxis(glm::vec3(0.0,0.0,1.0));
	//		bullets.push_back(std::move(bullet));
	//	}

	//	for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
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

		delayBetweenShootsTimer += dt;
		if (delayBetweenShootsTimer > delayBetweenShoots)
		{
			delayBetweenShootsTimer = 0.0f;

			switch (getShootingType())
			{
				case NORMAL:
				{
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));
					break;
				}
				case DOUBLE:
				{
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));
					break;
				}
				case HALFCIRCLE:
				{
					//Middle
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 20.0f, getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 40.0f, getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 20.0f, getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 40.0f, getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));
					break;
				}
				case DOUBLEHALFCIRCLE:
				{
					////////////1

					//Middle
					auto bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 20.0f - 4.5f - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 20.0f, getPosition(1) - 20.0f + 4.5f - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 40.0f, getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 20.0f, getPosition(1) - 20.0f - 4.5f - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 40.0f, getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					////////////2

					//Middle
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f), getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 20.0f, getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) - 40.0f, getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 20.0f, getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));

					bullet = std::make_shared<Bullet>(9.0f, 20.0f, glm::vec2(getPosition(0) + (getSize(0) / 2.0f) + 40.0f, getPosition(1) - 4.5f), glm::vec2(0.0f, -200.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", getAnimationByIndex("explosion"));
					bullet->applyAnimation(getAnimationByIndex("shoot"));
					bullet->setRotationAngle((float)M_PI);
					bullet->setRotationAxis(glm::vec3(0.0, 0.0, 1.0));
					addBullet(std::move(bullet));
					break;
				}
			}
		}

		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			if ((*it)->update(dt))
				it = bullets.erase(it);
			else
				++it;
		}

		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end();)
		{
			if ((*it).second->update(dt, position))
				it = addons.erase(it);
			else
				++it;
		}

		return getNeedsToBeDeleted();
	}

	void Enemy::onCollision(BaseGameObject* collider)
	{
		if (getAddon("shield") != nullptr)
			removeAddon("shield");
		else
			setNeedsToBeDeleted(true);

		if (collider != nullptr && !collider->getNeedsToBeDeleted())
			collider->onCollision(this);
	}


	std::shared_ptr<Addon> Enemy::getAddon(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
				return it->second;
		}
		return nullptr;
	}

	void Enemy::removeAddon(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
			{
				addons.erase(it);
				return;
			}
		}
	}

	void Enemy::addAddon(std::pair<std::string, std::shared_ptr<Addon>> _addon)
	{
		for (auto addon : addons)
		{
			if (addon.first == _addon.first) return;
		}

		addons.push_back(_addon);
	}
}