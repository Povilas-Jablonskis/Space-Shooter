#include "Addon.h"

namespace Engine
{
	Addon::Addon(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color) :
		BaseGameObject(_width, _height, glm::vec2(0.0, 0.0), _velocity, _color), positionOffset(_position)
	{

	}

	bool Addon::update(float dt, glm::vec2 playerPosition)
	{
		position = playerPosition + positionOffset;
		updateTexture(dt);
		return needsToBeDeleted;
	}
}