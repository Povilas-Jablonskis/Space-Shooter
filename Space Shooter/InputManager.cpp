#include "InputManager.hpp"
#include "KeyBinding.hpp"

#include <algorithm>
#include <glew/glew.h>
#include <freeglut/freeglut.h>
#include <system_error>
#include "rapidxml/RapidXMLSTD.hpp"

bool InputManager::getLeftMouseState() const
{
	return m_leftMouseClick;
}

bool InputManager::getRightMouseState() const
{
	return m_rightMouseClick;
}

void InputManager::setLeftMouseState(const bool boolean)
{
	m_leftMouseClick = boolean;
}

void InputManager::setRightMouseState(const bool boolean)
{
	m_rightMouseClick = boolean;
}

bool InputManager::getLastLeftMouseState() const
{
	return m_lastLeftMouseClick;
}

bool InputManager::getLastRightMouseState() const
{
	return m_lastRightMouseClick;
}

void InputManager::setLastLeftMouseState(const bool boolean)
{
	m_lastLeftMouseClick = boolean;
}

void InputManager::setLastRightMouseState(const bool boolean)
{
	m_lastRightMouseClick = boolean;
}

void InputManager::setLastMousePosition(const glm::vec2& position)
{
	m_lastMousePosition = position;
}

const glm::vec2& InputManager::getLastMousePosition() const
{
	return m_lastMousePosition;
}

bool InputManager::getKey(const short key) const
{
	try
	{
		return m_keyStates.at(key);
	}
	catch (const std::exception&)
	{
		return false;
	}
}

bool InputManager::getKey(const std::string& key) const
{
	const auto foundKb = std::ranges::find_if(m_keyBindings, [=](const std::shared_ptr<KeyBinding>& kb)
	{
		return kb->getKeyBinding() == key;
	});

	return foundKb != m_keyBindings.end() ? getKey((*foundKb)->getKeyBindingCharacter()) : false;
}

void InputManager::setKey(const short key, bool boolean)
{
	m_keyStates.insert_or_assign(key, boolean);
}

std::unordered_map<short, bool>& InputManager::getKeys()
{
	return m_keyStates;
}

std::vector<std::shared_ptr<KeyBinding>>& InputManager::getKeyBindings()
{
	return m_keyBindings;
}

void InputManager::addKeyBinding(const std::shared_ptr<KeyBinding>& key_binding)
{
	m_keyBindings.push_back(key_binding);
}

std::shared_ptr<KeyBinding> InputManager::getCurrentlyEditedKeyBinding() const
{
	const auto foundKb = std::ranges::find_if(m_keyBindings, [=](const std::shared_ptr<KeyBinding>& kb)
	{
		return kb->isCurrentlyEdited();
	});
	return foundKb != m_keyBindings.end() ? (*foundKb) : nullptr;
}

void InputManager::motionFunc(const int x, const int y)
{
	auto lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
	lastMousePosition.y = std::abs(lastMousePosition.y);
	setLastMousePosition(lastMousePosition);
}

void InputManager::processMouseClick(const int button, const int state, const int x, const int y)
{
	auto lastMousePosition = glm::vec2(x, y);
	lastMousePosition.y -= static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
	lastMousePosition.y = std::abs(lastMousePosition.y);

	if (button == GLUT_LEFT_BUTTON)
	{
		setLastLeftMouseState(getLeftMouseState());
		setLeftMouseState(state == GLUT_DOWN ? true : false);
		setLastMousePosition(lastMousePosition);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		setLastRightMouseState(getRightMouseState());
		setRightMouseState(state == GLUT_DOWN ? true : false);
		setLastMousePosition(lastMousePosition);
	}
}

void InputManager::keyboardInput(const char c)
{
	const auto key = VkKeyScan(c);
	if (!getKey(key))
	{
		setKey(key, true);
	}
}

void InputManager::keyboardInputUp(const char c, int x, int y)
{
	const auto key = VkKeyScan(c);
	if (getKey(key))
	{
		setKey(key, false);
	}
}

void InputManager::specialKeyInput(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (getKey(VK_UP))
			{
				setKey(VK_UP, false);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (getKey(VK_DOWN))
			{
				setKey(VK_DOWN, false);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (getKey(VK_LEFT))
			{
				setKey(VK_LEFT, false);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (getKey(VK_RIGHT))
			{
				setKey(VK_RIGHT, false);
			}
			break;
		}
	default: ;
	}
}

void InputManager::specialKeyInputUp(const int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (!getKey(VK_UP))
			{
				setKey(VK_UP, true);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (!getKey(VK_DOWN))
			{
				setKey(VK_DOWN, true);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (!getKey(VK_LEFT))
			{
				setKey(VK_LEFT, true);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (!getKey(VK_RIGHT))
			{
				setKey(VK_RIGHT, true);
			}
			break;
		}
	default: ;
	}
}

void InputManager::clearEverything()
{
	m_lastMousePosition = glm::vec2(0.0f, 0.0f);
	m_lastLeftMouseClick = false;
	m_leftMouseClick = false;
	m_lastRightMouseClick = false;
	m_rightMouseClick = false;
	m_keyStates.clear();
}
