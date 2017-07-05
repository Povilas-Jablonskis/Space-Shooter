#ifndef _playerH
#define _playerH

#include <map>

#include "Application.h"
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
			bool Update(std::vector<std::shared_ptr<Bullet>>*, const Application*);
			int GetHealth() const;
			int GetScore() const;
		private:
			int score;
			int health;
	};
}
#endif