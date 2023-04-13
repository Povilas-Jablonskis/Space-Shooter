#include "Keybind.hpp"

void Keybind::set(BindableAction key, short value)
{
	m_keyBindPair = KeybindPair(key, value);
}

KeybindPair Keybind::get() const
{
	return m_keyBindPair;
}

bool Keybind::isCurrentlyEdited() const
{
	return m_currentlyEdited;
}

void Keybind::setCurrentlyEdited(const bool boolean)
{
	m_currentlyEdited = boolean;
}

const std::shared_ptr<Text>& Keybind::getText() const
{
	return m_text;
}

void Keybind::setText(const std::shared_ptr<Text>& text)
{
	m_text = text;
}