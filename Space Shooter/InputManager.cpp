#include "InputManager.hpp"
#include "KeyBinding.hpp"

#include <iostream>
#include <algorithm>
#include <system_error>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

bool InputManager::getKey(const short key) 
{ 
	try
	{
		return m_keyStates.at(key);
	}
	catch (const std::exception&)
	{
		return false;
	};
}

bool InputManager::getKey(const std::string& key)
{
	auto& keyBindings = *getKeyBindings();
	const auto kb = std::find_if(keyBindings.begin(), keyBindings.end(), [key](auto kb) { return kb->getKeyBinding() == key; });
		
	return kb != keyBindings.end() ? getKey((*kb)->getKeyBindingCharacter()) : false;
}

void InputManager::setKey(const short key, bool boolean)
{
	m_keyStates.insert_or_assign(key, boolean);
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

void InputManager::keyboardInput(unsigned char c)
{
	const auto key = VkKeyScan(c);
	if (!getKey(key))
	{
		setKey(key, true);
	}
}


void InputManager::keyboardInputUp(const unsigned char c, int x, int y)
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

std::string InputManager::virtualKeyCodeToString(const int virtualKey)
{
	auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	TCHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_RCONTROL: case VK_RMENU:
		case VK_LWIN: case VK_RWIN: case VK_APPS:
		case VK_PRIOR: case VK_NEXT:
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:
		case VK_NUMLOCK:
			scanCode |= KF_EXTENDED;
			break;
		default:
			result = GetKeyNameText(scanCode << 16, szName, 128);
			break;
	}
	if (result == 0)
		throw std::system_error(std::error_code(GetLastError(), std::system_category()),
			"WinAPI Error occured.");
	return szName;
}