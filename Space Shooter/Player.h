#ifndef _playerH
#define _playerH

#include "BaseGameObject.h"
#include "Bullet.h"
#include "Subject.h"
#include "Addon.h"

namespace Engine
{
	class Player : public BaseGameObject, public Subject
	{
		public:
			~Player();
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			void respawn();
			void restart();
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notifyBase(ObserverEvent::HEALTHCHANGED); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notifyBase(ObserverEvent::SCORECHANGED); }
			void onCollision(BaseGameObject*);
		private:
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif