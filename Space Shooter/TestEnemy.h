#ifndef _testEnemyH
#define _testEnemyH

#include "BaseGameObject.h"

namespace Engine
{
	class TestEnemy : public BaseGameObject
	{
		public:
			~TestEnemy();
			TestEnemy(int, int, glm::vec2, glm::vec2, glm::vec3);
			TestEnemy();
			bool Update(float, float);
			GLboolean CheckCollision(std::shared_ptr<BaseGameObject>);
			GLboolean CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void OnCollision(std::shared_ptr<BaseGameObject>);
	};
}
#endif