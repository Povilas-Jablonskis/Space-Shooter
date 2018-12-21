#ifndef _bulletH
#define _bulletH

#include <vec2.hpp>
#include <string>>
#include <vector>

namespace Engine
{
	struct Bullet
	{
		glm::vec2 positionOffset;
		glm::vec2 velocity;
		std::string spriteName;
		std::string explosionSound;
		std::vector<std::pair<std::string, std::string>> sprites;
		std::vector<std::pair<std::string, std::string>> animations;
	};
}
#endif