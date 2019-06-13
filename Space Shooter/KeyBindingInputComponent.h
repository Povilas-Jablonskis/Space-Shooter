#ifndef _keyBindingInputComponentH
#define _keyBindingInputComponentH

#include "InputManager.h"

namespace Engine
{
	class Text;
	class KeyBindingInputComponent
	{
		public:
			KeyBindingInputComponent(std::string);
			void update(Text*, std::shared_ptr<InputManager>);
			static std::string virtualKeyCodeToString(int);
		private:
			std::string keyBinding;
	};
}
#endif