#ifndef _Bullet
#define _Bullet

#include "BaseGameObject.h"

namespace Engine
{
	class Bullet : public BaseGameObject
	{
		public:
			~Bullet();
			Bullet(int, int, float, float, float, float, float, float, float);
			Bullet();
			std::vector<std::shared_ptr<BaseGameObject>>::iterator Update(std::vector<std::shared_ptr<BaseGameObject>>&, std::vector<std::shared_ptr<BaseGameObject>>::iterator);
	};
}
#endif