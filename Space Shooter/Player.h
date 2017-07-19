#ifndef _playerH
#define _playerH

#include <map>

#include "Application.h"
#include "BaseGameObject.h"
#include "UIElement.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			Player(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Player();
			bool update(InputManager*, float);
			int getHealth() const;
			void reset();
			void setHealth(int);
			int getScore() const;
			void setScore(int);
			void onCollision(BaseGameObject*);
		private:
			glm::vec2 startVelocity;
			int score;
			int health;
	};
}
#endif