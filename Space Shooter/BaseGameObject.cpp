#include "BaseGameObject.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: RenderObject(_width, _height, _position, _color), velocity(_velocity), delayBetweenShoots(0.1f), delayBetweenShootsTimer(0.0f)
	{

	}

	BaseGameObject::~BaseGameObject()
	{

	}

	bool BaseGameObject::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);
		return true;
	}

	void BaseGameObject::onCollision(BaseGameObject* object, BaseGameObject* collider, CollisionType type)
	{
		std::cout << "some gameobject hit" << std::endl;
	}
}