#ifndef keyBindingInputComponentH
#define keyBindingInputComponentH

#include "Text.hpp"

#include <string>

class Text;

class KeyBinding
{
public:
	KeyBinding(std::string, short, std::shared_ptr<Text>);
	[[nodiscard]] const std::string& getKeyBinding() const;
	void setKeyBinding(const std::string&);
	[[nodiscard]] short getKeyBindingCharacter() const;
	void setKeyBindingCharacter(short);
	[[nodiscard]] bool isCurrentlyEdited() const;
	void setCurrentlyEdited(bool);
	[[nodiscard]] const std::shared_ptr<Text>& getText() const;

private:
	short m_keyBindingCharacter{};
	std::string m_keyBinding{};
	bool m_currentlyEdited{};

	std::shared_ptr<Text> m_text;
};
#endif
