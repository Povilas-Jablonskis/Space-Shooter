#ifndef _playerH
#define _playerH

#include "Entity.h"

namespace Engine
{
	class Player : public Entity
	{
		public:
			~Player();
			Player(float, float, glm::vec2, glm::vec2, glm::vec4);
			bool update(float);
			inline int getHealth() const { return health; }
			inline void setHealth(int _health) { health = _health; notify(ObserverEvent::HEALTHCHANGED, std::map<std::string, BaseGameObject*>()); }
			inline int getScore() const { return score; }
			inline void setScore(int _score) { score = _score; notify(ObserverEvent::SCORECHANGED, std::map<std::string, BaseGameObject*>()); }
			void addBullet(std::shared_ptr<BaseGameObject>);
		private:
			void respawn();
			glm::vec2 startVelocity;
			int score;
			int startHealth;
			int health;
	};
}
#endif