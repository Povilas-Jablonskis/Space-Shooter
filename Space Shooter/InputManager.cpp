#include "InputManager.hpp"
#include "MenuManager.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"

#include <algorithm>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

namespace Engine
{
	bool InputManager::getKey(char key) 
	{ 
		auto keyStates = *getKeys();
		const auto it = std::find_if(keyStates.begin(), keyStates.end(), [key](auto kb) { return kb.first == key; });

		return it != keyStates.end() ? it->second : false;
	}

	bool InputManager::getLastKey(char key)
	{
		auto lastKeyStates = *getLastKeys();
		const auto it = std::find_if(lastKeyStates.begin(), lastKeyStates.end(), [key](auto kb) { return kb.first == key; });

		return it != lastKeyStates.end() ? it->second : false;
	}

	bool InputManager::getKey(const std::string& key)
	{
		auto keyBindings = *getKeyBindings();
		const auto kb = std::find_if(keyBindings.begin(), keyBindings.end(), [key](auto kb) { return kb.first == key; });

		return kb != keyBindings.end() ? getKey(kb->second) : false;
	}

	bool InputManager::getLastKey(const std::string& key)
	{
		auto keyBindings = *getKeyBindings();
		const auto kb = std::find_if(keyBindings.begin(), keyBindings.end(), [key](auto kb) { return kb.first == key; });

		return kb != keyBindings.end() ? getLastKey(kb->second) : false;
	}

	void InputManager::setKeyBinding(const keyBinding& pair)
	{
		auto keyBindings = getKeyBindings();
		for (auto it = keyBindings->begin(); it != keyBindings->end(); ++it)
		{
			if ((*it).first == pair.first)
			{
				keyBindings->erase(it);
				break;
			}
		}

		keyBindings->push_back(pair);
	}

	void InputManager::setKey(char key, bool boolean)
	{
		auto keyStates = getKeys();
		for (auto it = keyStates->begin(); it != keyStates->end(); ++it)
		{
			if ((*it).first == key)
			{
				keyStates->erase(it);
				break;
			}
		}

		keyStates->push_back(keyState(key, boolean));
	}

	void InputManager::setLastKey(char key, bool boolean)
	{
		auto lastKeyStates = getLastKeys();
		for (auto it = lastKeyStates->begin(); it != lastKeyStates->end(); ++it)
		{
			if ((*it).first == key)
			{
				lastKeyStates->erase(it);
				break;
			}
		}

		lastKeyStates->push_back(keyState(key, boolean));
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

	void InputManager::keyboardInput(unsigned char c, int x, int y, const std::shared_ptr<MenuManager>& menuManager, irrklang::ISoundEngine* soundEngine, const std::shared_ptr<GameStateManager>& gameStateManager, const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		const auto key = VkKeyScan(c);
		if (!getKey(key))
		{
			switch (c)
			{
				case 27: // escape
				{
					menuManager->escapeAction(soundEngine, this, gameStateManager, spriteSheetManager);
					break;
				}
			}

			setKey(key, true);
			setLastKey(key, false);
		}
	}


	void InputManager::keyboardInputUp(const unsigned char c, int x, int y)
	{
		const auto key = VkKeyScan(c);
		if (getKey(key))
		{
			setKey(key, false);
			setLastKey(key, true);
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
					setLastKey(VK_UP, true);
				}
				break;
			}
			case GLUT_KEY_DOWN:
			{
				if (getKey(VK_DOWN))
				{
					setKey(VK_DOWN, false);
					setLastKey(VK_DOWN, true);
				}
				break;
			}
			case GLUT_KEY_LEFT:
			{
				if (getKey(VK_LEFT))
				{
					setKey(VK_LEFT, false);
					setLastKey(VK_LEFT, true);
				}
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				if (getKey(VK_RIGHT))
				{
					setKey(VK_RIGHT, false);
					setLastKey(VK_RIGHT, true);
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
					setLastKey(VK_UP, false);
				}
				break;
			}
			case GLUT_KEY_DOWN:
			{
				if (!getKey(VK_DOWN))
				{
					setKey(VK_DOWN, true);
					setLastKey(VK_DOWN, false);
				}
				break;
			}
			case GLUT_KEY_LEFT:
			{
				if (!getKey(VK_LEFT))
				{
					setKey(VK_LEFT, true);
					setLastKey(VK_LEFT, false);
				}
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				if (!getKey(VK_RIGHT))
				{
					setKey(VK_RIGHT, true);
					setLastKey(VK_RIGHT, false);
				}
				break;
			}
		}
	}

	std::string InputManager::virtualKeyCodeToString(const int virtualKey)
	{
		auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		TCHAR szName[128];
		int result;
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
			default:
				result = GetKeyNameText(scanCode << 16, szName, 128);
		}
		if (result == 0)
			throw std::system_error(std::error_code(GetLastError(), std::system_category()),
				"WinAPI Error occured.");
		return szName;
	}
}