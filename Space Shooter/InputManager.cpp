#include "InputManager.hpp"

#include <glew/glew.h>
#include <freeglut/freeglut.h>

bool InputManager::isLeftMouseClicked() const
{
	return m_leftMouseClick;
}

bool InputManager::isRightMouseClicked() const
{
	return m_rightMouseClick;
}

void InputManager::setLeftMouseClicked(const bool leftMouseClick)
{
	m_leftMouseClick = leftMouseClick;
}

void InputManager::setRightMouseClicked(const bool rightMouseClick)
{
	m_rightMouseClick = rightMouseClick;
}

bool InputManager::isLastLeftMouseStateClicked() const
{
	return m_lastLeftMouseClick;
}

bool InputManager::isLastRightMouseStateClicked() const
{
	return m_lastRightMouseClick;
}

void InputManager::setLastLeftMouseClicked(const bool lastLeftMouseClick)
{
	m_lastLeftMouseClick = lastLeftMouseClick;
}

void InputManager::setLastRightMouseClicked(const bool lastRightMouseClick)
{
	m_lastRightMouseClick = lastRightMouseClick;
}

void InputManager::setLastMousePosition(const glm::vec2& lastMousePosition)
{
	m_lastMousePosition = lastMousePosition;
}

const glm::vec2& InputManager::getLastMousePosition() const
{
	return m_lastMousePosition;
}

bool InputManager::isKeyActive(const short key) const
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

bool InputManager::isKeyActive(const std::string& key) const
{
	const auto foundKb = std::ranges::find_if(m_keyBindings, [=](const std::shared_ptr<KeyBinding>& kb)
	{
		return kb->getKeyBinding() == key;
	});

	return foundKb != m_keyBindings.end() ? isKeyActive((*foundKb)->getKeyBindingCharacter()) : false;
}

void InputManager::setKeyState(const short key, bool boolean)
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

void InputManager::addKeyBinding(const std::shared_ptr<KeyBinding>& keyBinding)
{
	m_keyBindings.push_back(keyBinding);
}

std::shared_ptr<KeyBinding> InputManager::getCurrentlyEditedKeyBinding() const
{
	const auto foundKb = std::ranges::find_if(m_keyBindings, [=](const std::shared_ptr<KeyBinding>& kb)
	{
		return kb->isCurrentlyEdited();
	});
	return foundKb != m_keyBindings.end() ? *foundKb : nullptr;
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
		setLastLeftMouseClicked(isLeftMouseClicked());
		setLeftMouseClicked(state == GLUT_DOWN ? true : false);
		setLastMousePosition(lastMousePosition);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		setLastRightMouseClicked(isRightMouseClicked());
		setRightMouseClicked(state == GLUT_DOWN ? true : false);
		setLastMousePosition(lastMousePosition);
	}
}

void InputManager::keyboardInput(const char c)
{
	const auto key = VkKeyScan(c);
	if (!isKeyActive(key))
	{
		setKeyState(key, true);
	}
}

void InputManager::keyboardInputUp(const char c, int, int)
{
	const auto key = VkKeyScan(c);
	if (isKeyActive(key))
	{
		setKeyState(key, false);
	}
}

void InputManager::specialKeyInput(const int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (isKeyActive(VK_UP))
			{
				setKeyState(VK_UP, false);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (isKeyActive(VK_DOWN))
			{
				setKeyState(VK_DOWN, false);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (isKeyActive(VK_LEFT))
			{
				setKeyState(VK_LEFT, false);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (isKeyActive(VK_RIGHT))
			{
				setKeyState(VK_RIGHT, false);
			}
			break;
		}
	default: ;
	}
}

void InputManager::specialKeyInputUp(const int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		{
			if (!isKeyActive(VK_UP))
			{
				setKeyState(VK_UP, true);
			}
			break;
		}
	case GLUT_KEY_DOWN:
		{
			if (!isKeyActive(VK_DOWN))
			{
				setKeyState(VK_DOWN, true);
			}
			break;
		}
	case GLUT_KEY_LEFT:
		{
			if (!isKeyActive(VK_LEFT))
			{
				setKeyState(VK_LEFT, true);
			}
			break;
		}
	case GLUT_KEY_RIGHT:
		{
			if (!isKeyActive(VK_RIGHT))
			{
				setKeyState(VK_RIGHT, true);
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

void InputManager::checkInteraction(const std::shared_ptr<Text>& text) const
{
	if (!text->isActive())
	{
		return;
	}

	if (getLastMousePosition().x >= text->getBoundingBox().x && getLastMousePosition().x <=
		text->getBoundingBox().y && getLastMousePosition().y <= text->getBoundingBox().z &&
		getLastMousePosition().y >= text->getBoundingBox().a)
	{
		if (!text->isClickedByMouse())
		{
			if (!isLastLeftMouseStateClicked() && isLeftMouseClicked())
			{
				text->onMouseClickFunc();
				text->setMousedClicked(true);
			}
		}
		else
		{
			if (isLastLeftMouseStateClicked() && !isLeftMouseClicked())
			{
				text->onMouseReleaseFunc();
				text->setMousedClicked(false);
			}
		}

		if (!text->isHoveredByMouse())
		{
			text->onHoverEnterFuncDefaults();
			text->onHoverEnterFunc();
			text->setMousedHovered(true);
		}
	}
	else
	{
		if (text->isHoveredByMouse())
		{
			text->onHoverExitFuncDefaults();
			text->onHoverExitFunc();
			text->setMousedHovered(false);
		}
	}
}
