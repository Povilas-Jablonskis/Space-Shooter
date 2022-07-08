#include "KeyBinding.hpp"
#include "FileConstants.hpp"
#include "Text.hpp"

#include <algorithm>
#include <utility>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

KeyBinding::KeyBinding(const std::string& keyBinding, short keyBindingCharacter, const std::shared_ptr<Text>& text) : m_keyBindingCharacter(keyBindingCharacter), m_keyBinding(keyBinding), m_text(text)
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

void KeyBinding::setKeyBindingCharacter(short keyBindingCharacter)
{
	m_keyBindingCharacter = keyBindingCharacter;
}

bool KeyBinding::isCurrentlyEdited() const
{
	return m_currentlyEdited;
}

void KeyBinding::setCurrentlyEdited(bool boolean)
{
	m_currentlyEdited = boolean;
}

const std::shared_ptr<Text>& KeyBinding::getText() const
{
	return m_text;
}