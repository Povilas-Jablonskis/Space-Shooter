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
			Player(int, int, glm::vec2, glm::vec2, glm::vec4, std::shared_ptr<Application>);
			~Player();
			bool update(float);
			void reset();
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; }
			inline int getLastHealth() const { return lastHealth; }
			inline void setLastHealth(int _health) { lastHealth = _health; }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; }
			inline int getLastScore() const { return lastScore; }
			inline void setLastScore(int _score) { lastScore = _score; }
			void onCollision(BaseGameObject*);
		private:
			glm::vec2 startVelocity;
			int lastScore;
			int score;
			int lastHealth;
			int health;
	};
}
#endif