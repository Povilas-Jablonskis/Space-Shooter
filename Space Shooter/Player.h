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
			bool Update(Application*, float);
			int GetHealth() const;
			int GetScore() const;
			GLboolean CheckCollision(std::shared_ptr<BaseGameObject>);
			GLboolean CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void OnCollision(std::shared_ptr<BaseGameObject>);
		private:
			int score;
			int health;
	};
}
#endif