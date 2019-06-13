#ifndef _uiinputComponentH
#define _uiinputComponentH

#include "InputManager.h"

namespace Engine
{
	class UIElementBase;
	class UIInputComponent
	{
		public:
			void update(UIElementBase*, std::shared_ptr<InputManager>);
	};
}
#endif