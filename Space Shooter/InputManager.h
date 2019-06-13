#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"
#include <map>
#include <vector>
#include <string>

namespace Engine
{
	class InputManager
	{
		public:
			InputManager();
			inline bool getLeftMouseState() { return leftMouseClick; }
			inline bool getRightMouseState() { return rightMouseClick; }
			inline void setLeftMouseState(bool boolean) { leftMouseClick = boolean; }
			inline void setRightMouseState(bool boolean) { rightMouseClick = boolean; }
			inline bool getLastLeftMouseState() { return lastLeftMouseClick; }
			inline bool getLastRightMouseState() { return lastRightMouseClick; }
			inline void setLastLeftMouseState(bool boolean) { lastLeftMouseClick = boolean; }
			inline void setLastRightMouseState(bool boolean) { lastRightMouseClick = boolean; }
			inline void setLastMousePosition(glm::vec2 position) { lastMousePosition = position; }
			inline glm::vec2 getLastMousePosition() { return lastMousePosition; }
			inline bool getKey(char key) { return keyStates[key]; }
			inline bool getKey(int key) { return keyStates[key]; }
			inline bool getKey(std::string keyBinding) { return keyStates[keyBindings[keyBinding]]; }
			inline bool getLastKey(char key) { return lastKeyStates[key]; }
			inline bool getLastKey(int key) { return lastKeyStates[key]; }
			inline bool getLastKey(std::string keyBinding) { return lastKeyStates[keyBindings[keyBinding]]; }
			inline void setKey(char key, bool boolean) { keyStates[key] = boolean; }
			inline void setKey(int key, bool boolean) { keyStates[key] = boolean; }
			inline void setLastKey(char key, bool boolean) { lastKeyStates[key] = boolean; }
			inline void setLastKey(int key, bool boolean) { lastKeyStates[key] = boolean; }
			inline std::map<int, bool> getKeys() { return keyStates; }
			inline std::map<std::string, int> getKeyBindings() { return keyBindings; }
			void setKeyBinding(std::pair<std::string, int>);
			inline std::string getCurrentlyEditedKeyBinding() { return currentlyEditedKeyBinding; }
			inline void setCurrentlyEditedKeyBinding(std::string str) { currentlyEditedKeyBinding = str; }
			void update();
		private:
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
			std::map<int, bool> keyStates;
			std::map<int, bool> lastKeyStates;
			std::map<std::string, int> keyBindings;
			std::string currentlyEditedKeyBinding;
	};
}
#endif