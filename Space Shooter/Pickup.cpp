#include "Pickup.h"

namespace Engine
{
	Pickup::Pickup(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color) :
		BaseGameObject(_width, _height, glm::vec2(_position), _velocity, _color)
	{
		effect = std::function<void(Player*)>
		(
			
		);
	}

	bool Pickup::update(float dt)
	{
		updateAnimation(dt);

		return needsToBeDeleted;
	}

	void Pickup::onCollision(Player* player)
	{
		if (player->getNeedsToBeDeleted()) return;

		effect(player);
		needsToBeDeleted = true;

		#if _DEBUG
			std::cout << "pickup player hit" << std::endl;
		#endif
	}
}