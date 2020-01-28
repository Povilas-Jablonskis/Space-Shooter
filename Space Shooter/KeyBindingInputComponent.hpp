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
			explicit KeyBindingInputComponent(std::string);
			void update(Text*, const std::shared_ptr<InputManager>&) const;
		private:
			void savePlayerConfig(const std::shared_ptr<InputManager>&) const;
			const std::string& getKeyBinding() const { return m_keyBinding; }

			std::string m_keyBinding;
	};
}
#endif