#include "BaseGameObject.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), delayBetweenShoots(0.2f), delayBetweenShootsTimer(0.0f), shootingType(ShootingType::NORMAL)
	{
		onDeath = []()
		{

		};
	}

	BaseGameObject::~BaseGameObject()
	{
		animations.clear();
	}

	bool BaseGameObject::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateAnimation(_dt);
		return needsToBeDeleted;
	}

	void BaseGameObject::onCollision(BaseGameObject* collider)
	{
		#if _DEBUG
			std::cout << "some gameobject hit" << std::endl;
		#endif
	}

	void BaseGameObject::addAnimation(std::string index, std::shared_ptr<Animation> animation)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Animation>>>::iterator it = animations.begin(); it != animations.end(); it++)
		{
			if (it->first == index)
				return;
		}

		animations.push_back(std::pair<std::string, std::shared_ptr<Animation>>(index, animation));
	}

	std::shared_ptr<Addon> BaseGameObject::getAddon(std::string index)
	{
		for (std::vector<std::pair<std::string, std::shared_ptr<Addon>>>::iterator it = addons.begin(); it != addons.end(); it++)
		{
			if (it->first == index)
				return it->second;
		}
		return nullptr;
	}

	void BaseGameObject::removeAddon(std::string index)
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

	void BaseGameObject::deleteBullet(Bullet* bullet)
	{
		for (std::vector<std::shared_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end(); it++)
		{
			if (it->get() == bullet)
			{
				(*it)->setNeedsToBeDeleted(true);
				return;
			}
		}
	}

	std::shared_ptr<Animation> BaseGameObject::getAnimationByIndex(std::string index)
	{
		for (auto animation : animations)
		{
			if (animation.first == index)
				return animation.second;
		}
	}

	void BaseGameObject::addAddon(std::pair<std::string, std::shared_ptr<Addon>> _addon)
	{
		for (auto addon : addons)
		{
			if (addon.first == _addon.first) return;
		}

		addons.push_back(_addon);
	}
}