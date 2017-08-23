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
		if (animations.find(index) != animations.end())
			return;

		animations.insert(std::pair<std::string, std::shared_ptr<Animation>>(index, animation));
	}

	std::shared_ptr<Addon> BaseGameObject::getAddon(std::string index)
	{
		auto addon = addons.find(index);
		if (addon != addons.end())
			return addon->second;
		return nullptr;
	}

	void BaseGameObject::removeAddon(std::string index)
	{
		auto addon = addons.find(index);
		if (addon != addons.end())
			addons.erase(addon);
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
}