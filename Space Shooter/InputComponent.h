#ifndef _inputComponentH
#define _inputComponentH

#include "InputManager.h"

namespace Engine
{
	class Entity;
	class InputComponent
	{
		public:
			void update(Entity*, std::shared_ptr<InputManager>, float);
	};
}
#endif