#ifndef _testEnemyH
#define _testEnemyH

#include "BaseGameObject.h"

namespace Engine
{
	class TestEnemy : public BaseGameObject
	{
		public:
			~TestEnemy();
			TestEnemy(int, int, glm::vec2, glm::vec2, glm::vec4);
			bool update(float, float);
			GLboolean checkCollision(std::shared_ptr<BaseGameObject>);
			GLboolean checkCollision(std::vector<std::shared_ptr<BaseGameObject>>*);
			void onCollision(std::shared_ptr<BaseGameObject>);
	};
}
#endif