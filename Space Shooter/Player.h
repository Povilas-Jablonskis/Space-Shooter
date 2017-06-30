#ifndef _Player
#define _Player

#include <map>

#include "Bullet.h"
#include "UIElement.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			~Player();
			Player(int, int, glm::vec2, glm::vec2, glm::vec3);
			Player();
			bool Update(std::vector<std::shared_ptr<Bullet>>*);
			const int GetHealth();
			const int GetScore();
		private:
			bool pressedkeys[256];
			int score;
			int health;
	};
}
#endif