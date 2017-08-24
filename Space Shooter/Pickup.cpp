#include "Pickup.h"

namespace Engine
{
	Pickup::Pickup(float _width, float _height, glm::vec2 _position) :
		RenderObject(_width, _height, glm::vec2(_position), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f))
	{
		effect = std::function<void(BaseGameObject*)>
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