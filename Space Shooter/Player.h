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
			Player(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Player();
			bool update(InputManager*, float, std::vector<std::shared_ptr<Bullet>>*);
			int getHealth() const;
			void setHealth(int);
			int getScore() const;
			void setScore(int);
			void onCollision(BaseGameObject*);
		private:
			int score;
			int health;
	};
}
#endif