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
			glm::vec2 getStartVelocity() { return startVelocity; }
			glm::vec2 getStartPosition() { return startPosition; }
			inline void setLives(int _lives) { lives = _lives; notify(ObserverEvent::LIVES_CHANGED, std::vector<std::pair<std::string, BaseGameObject*>>()); }
			inline int getScore()  { return score; }
			inline void setScore(int _score) { score = _score; notify(ObserverEvent::SCORE_CHANGED, std::vector<std::pair<std::string, BaseGameObject*>>()); }
			inline std::string getLivesIcon() { return livesIcon; }
			inline void setLivesIcon(std::string _icon) { livesIcon = _icon; }
		private:
			std::string livesIcon;
			glm::vec2 startVelocity;
			glm::vec2 startPosition;
			int score;
	};
}
#endif