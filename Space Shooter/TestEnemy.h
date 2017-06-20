#ifndef _TestEnemy
#define _TestEnemy

#include "BaseGameObject.h"

namespace Engine
{
	class TestEnemy : public BaseGameObject
	{
		public:
			~TestEnemy();
			TestEnemy(int, int, float, float, float, float, float, float, float);
			TestEnemy();
			void Update(int);
		private:
			float dt;
	};
}
#endif