#include "Pickup.h"

namespace Engine
{
	Pickup::Pickup(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		effect = [](std::shared_ptr<BaseGameObject> collider)
		{
			return true;
		};
	}
}