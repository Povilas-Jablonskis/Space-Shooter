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
			bool Update();
		private:
			float dt;
	};
}
#endif