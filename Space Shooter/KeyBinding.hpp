#ifndef keyBindingInputComponentH
#define keyBindingInputComponentH

#include "InputManager.hpp"
#include "SharedContext.hpp"
#include "Text.hpp"

#include <string>

class Text;

class KeyBinding
{
public:
	KeyBinding(const std::string&, short, const std::shared_ptr<Text>&);
	const std::string& getKeyBinding() const;
	void setKeyBinding(const std::string&);
	short getKeyBindingCharacter() const;
	void setKeyBindingCharacter(short);
	bool isCurrentlyEdited() const;
	void setCurrentlyEdited(bool);
	const std::shared_ptr<Text>& getText() const;
private:
	short m_keyBindingCharacter{};
	std::string m_keyBinding{};
	bool m_currentlyEdited{};

	std::shared_ptr<Text> m_text;
};
#endif