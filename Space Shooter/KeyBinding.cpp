#include "KeyBinding.hpp"
#include "Text.hpp"

KeyBinding::KeyBinding(std::string keyBinding, const short keyBindingCharacter, std::shared_ptr<Text> text) :
	m_keyBindingCharacter(keyBindingCharacter), m_keyBinding(std::move(keyBinding)), m_text(std::move(text))
{
}

const std::string& KeyBinding::getKeyBinding() const
{
	return m_keyBinding;
}

void KeyBinding::setKeyBinding(const std::string& keyBinding)
{
	m_keyBinding = keyBinding;
}

short KeyBinding::getKeyBindingCharacter() const
{
	return m_keyBindingCharacter;
}

void KeyBinding::setKeyBindingCharacter(const short keyBindingCharacter)
{
	m_keyBindingCharacter = keyBindingCharacter;
}

bool KeyBinding::isCurrentlyEdited() const
{
	return m_currentlyEdited;
}

void KeyBinding::setCurrentlyEdited(const bool boolean)
{
	m_currentlyEdited = boolean;
}

const std::shared_ptr<Text>& KeyBinding::getText() const
{
	return m_text;
}
