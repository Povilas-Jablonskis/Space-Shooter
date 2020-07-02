#ifndef keyBindingInputComponentH
#define keyBindingInputComponentH

#include <string>
#include <memory>

namespace Engine
{
	class Text;
	class InputManager;

	class KeyBinding
	{
		public:
			explicit KeyBinding(std::string, char);
			void update(const std::shared_ptr<InputManager>&);
			const std::string& getKeyBinding() const { return m_keyBinding; }
			char getKeyBindingCharacter() const { return m_keyBindingCharacter; }
			void setText(const std::shared_ptr<Text>& text) { m_text = text; }
			std::shared_ptr<Text> getText() const { return m_text; }
		private:
			void setKeyBindingCharacter(char character) { m_keyBindingCharacter = character; }
			void savePlayerConfig(const std::shared_ptr<InputManager>&) const;

			char m_keyBindingCharacter;
			std::string m_keyBinding;

			std::shared_ptr<Text> m_text = nullptr;
	};
}
#endif