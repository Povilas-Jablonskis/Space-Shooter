#ifndef _Player
#define _Player

#include "BaseGameObject.h"

namespace Engine
{
	class Player : public BaseGameObject
	{
		public:
			~Player();
			Player(int, int, float, float, float, float, float, float, float);
			Player();
			void Update(int);
		private:
			bool pressedkeys[256];
	};
}
#endif