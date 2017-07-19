#ifndef _playerH
#define _playerH

#include <map>

#include "BaseGameObject.h"
#include "InputManager.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			Player(int, int, glm::vec2, glm::vec2, glm::vec4);
			~Player();
			bool update(InputManager*, float);
			inline int getHealth() const { return health; }
			void reset();
			inline void setHealth(int _health) { health = _health; }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; }
			void onCollision(BaseGameObject*);
		private:
			glm::vec2 startVelocity;
			int score;
			int health;
	};
}
#endif