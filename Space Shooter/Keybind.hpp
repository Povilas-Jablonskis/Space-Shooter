#ifndef keyBindH
#define keyBindH

#include "BindableAction.hpp"
#include "Text.hpp"

#include <memory>

using KeybindPair = std::pair<BindableAction, short>;

class Keybind
{
public:
	void set(BindableAction, short);
	[[nodiscard]] KeybindPair get() const;
	[[nodiscard]] bool isCurrentlyEdited() const;
	void setCurrentlyEdited(bool);
	[[nodiscard]] const std::shared_ptr<Text>& getText() const;
	void setText(const std::shared_ptr<Text>&);
private:
	KeybindPair m_keyBindPair{};
	bool m_currentlyEdited{};
	std::shared_ptr<Text> m_text;
};
#endif
