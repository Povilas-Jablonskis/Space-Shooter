#include "Pickup.h"

namespace Engine
{
	Pickup::Pickup(float _width, float _height, glm::vec2 _position) :
		RenderObject(_width, _height, glm::vec2(_position), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f)), needsToBeDeleted(false)
	{
		effect = std::function<void(Player*)>
		(
			
		);
	}

	bool Pickup::update(float dt)
	{
		updateAnimation(dt);

		return getNeedsToBeDeleted();
	}

	void Pickup::onCollision(Player* collider)
	{
		effect(collider);
		setNeedsToBeDeleted(true);
	}
}