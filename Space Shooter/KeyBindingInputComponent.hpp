#ifndef keyBindingInputComponentH
#define keyBindingInputComponentH

#include <string>

namespace Engine
{
	class Text;
	class InputManager;

	class KeyBindingInputComponent
	{
		public:
			KeyBindingInputComponent(const std::string&);
			void update(Text*, const std::unique_ptr<InputManager>&);
		private:
			inline const std::string& getKeyBinding() const { return m_keyBinding; }

			std::string m_keyBinding;
	};
}
#endif