#include "BaseGameObjectPlus.h"

namespace Engine
{
	BaseGameObjectPlus::BaseGameObjectPlus(float _width, float _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		onCollision = [this](Entity*)
		{

		};
	}
}