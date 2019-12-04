#include "InputManager.hpp"
#include "MenuManager.hpp"
#include "SpriteSheetManager.hpp"
#include "GameStateManager.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Engine
{
	bool InputManager::getKey(char key) 
	{ 
		auto keyStates = *getKeys();
		auto it = std::find_if(keyStates.begin(), keyStates.end(), [key](auto kb) { return kb.first == key; });

		return it != keyStates.end() ? it->second : false;
	}

	bool InputManager::getLastKey(char key)
	{
		auto lastKeyStates = *getLastKeys();
		auto it = std::find_if(lastKeyStates.begin(), lastKeyStates.end(), [key](auto kb) { return kb.first == key; });

		return it != lastKeyStates.end() ? it->second : false;
	}

	bool InputManager::getKey(const std::string& key)
	{
		auto keyBindings = *getKeyBindings();
		auto kb = std::find_if(keyBindings.begin(), keyBindings.end(), [key](auto kb) { return kb.first == key; });

		return kb != keyBindings.end() ? getKey(kb->second) : false;
	}

	bool InputManager::getLastKey(const std::string& key)
	{
		auto keyBindings = *getKeyBindings();
		auto kb = std::find_if(keyBindings.begin(), keyBindings.end(), [key](auto kb) { return kb.first == key; });

		return kb != keyBindings.end() ? getLastKey(kb->second) : false;
	}

	void InputManager::setKeyBinding(keyBinding pair)
	{
		auto keyBindings = getKeyBindings();
		for (auto it = keyBindings->begin(); it != keyBindings->end();)
		{
			if ((*it).first == pair.first)
			{
				keyBindings->erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		keyBindings->push_back(pair);
	}

	void InputManager::setKey(char key, bool boolean)
	{
		auto keyStates = getKeys();
		for (auto it = keyStates->begin(); it != keyStates->end();)
		{
			if ((*it).first == key)
			{
				keyStates->erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		keyStates->push_back(keyState(key, boolean));
	}

	void InputManager::setLastKey(char key, bool boolean)
	{
		auto lastKeyStates = getLastKeys();
		for (auto it = lastKeyStates->begin(); it != lastKeyStates->end();)
		{
			if ((*it).first == key)
			{
				lastKeyStates->erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		lastKeyStates->push_back(keyState(key, boolean));
	}

	void InputManager::motionFunc(int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
		lastMousePosition.y -= static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));
		lastMousePosition.y = std::abs(lastMousePosition.y);
		setLastMousePosition(lastMousePosition);
	}

	void InputManager::processMouseClick(int button, int state, int x, int y)
	{
		glm::vec2 lastMousePosition = glm::vec2(x, y);
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

	void InputManager::keyboardInput(unsigned char c, int x, int y, const std::unique_ptr<MenuManager>& menuManager, irrklang::ISoundEngine* soundEngine, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		auto key = VkKeyScan(c);
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


	void InputManager::keyboardInputUp(unsigned char c, int x, int y)
	{
		auto key = VkKeyScan(c);
		if (getKey(key))
		{
			setKey(key, false);
			setLastKey(key, true);
		}
	}

	void InputManager::specialKeyInput(int key, int x, int y)
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

	void InputManager::specialKeyInputUp(int key, int x, int y)
	{
		auto c = 0;
		switch (key)
		{
			case GLUT_KEY_UP:
			{
				c = VK_UP;
				if (!getKey(VK_UP))
				{
					setKey(VK_UP, true);
					setLastKey(VK_UP, false);
				}
				break;
			}
			case GLUT_KEY_DOWN:
			{
				c = VK_DOWN;
				if (!getKey(VK_DOWN))
				{
					setKey(VK_DOWN, true);
					setLastKey(VK_DOWN, false);
				}
				break;
			}
			case GLUT_KEY_LEFT:
			{
				c = VK_LEFT;
				if (!getKey(VK_LEFT))
				{
					setKey(VK_LEFT, true);
					setLastKey(VK_LEFT, false);
				}
				break;
			}
			case GLUT_KEY_RIGHT:
			{
				c = VK_RIGHT;
				if (!getKey(VK_RIGHT))
				{
					setKey(VK_RIGHT, true);
					setLastKey(VK_RIGHT, false);
				}
				break;
			}
		}
	}

	const std::string InputManager::virtualKeyCodeToString(int virtualKey)
	{
		auto scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

		CHAR szName[128];
		auto result = 0;
		switch (virtualKey)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
			case VK_PRIOR: case VK_NEXT:
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE:
			case VK_NUMLOCK:
				scanCode |= 0x100;
			default:
				result = GetKeyNameTextA(scanCode << 16, szName, 128);
		}
		return szName;
	}
}