#include "BaseGameObject.h"
#include "Player.h"
#include "Enemy.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), needsToBeDeleted(false)
	{
		onDeath = []()
		{

		};

		collisionEffect = [](BaseGameObject* collider)
		{

		};

		collisionEffectEntity = [](Entity* collider)
		{

		};
	}

	BaseGameObject::~BaseGameObject()
	{
		animations.clear();
	}

	void BaseGameObject::onCollision(BaseGameObject* collider)
	{
		setNeedsToBeDeleted(true);
		collisionEffect(collider);
	}

	void BaseGameObject::onCollision(Entity* collider)
	{
		setNeedsToBeDeleted(true);
		collisionEffectEntity(collider);
	}

	bool BaseGameObject::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateAnimation(_dt);
		return getNeedsToBeDeleted();
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

	std::shared_ptr<Animation> BaseGameObject::getAnimationByIndex(std::string index)
	{
		for (auto animation : animations)
		{
			if (animation.first == index)
				return animation.second;
		}
		return nullptr;
	}
}