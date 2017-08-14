#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"
#include "vec4.hpp"
#include <map>
#include <memory>
#include <iostream>

#include "Text.h"
#include "Player.h"

namespace Engine
{
	class InputManager
	{
		public:
			InputManager();
			void resetInput();
			void fixInput();
			void updatePlayerInput(Player*, float);
			inline bool getLeftMouseState() const { return leftMouseClick; }
			inline bool getRightMouseState() const { return rightMouseClick; }
			inline void setLeftMouseState(bool boolean) { leftMouseClick = boolean; }
			inline void setRightMouseState(bool boolean) { rightMouseClick = boolean; }
			inline bool getLastLeftMouseState() const { return lastLeftMouseClick; }
			inline bool getLastRightMouseState() const { return lastRightMouseClick; }
			inline void setLastLeftMouseState(bool boolean) { lastLeftMouseClick = boolean; }
			inline void setLastRightMouseState(bool boolean) { lastRightMouseClick = boolean; }
			inline bool getKey(char key) const { return pressedkeys[key]; }
			inline bool getKey(int key) const { return pressedkeys[key]; }
			inline void setKey(char key, bool boolean) { pressedkeys[key] = boolean; }
			inline void setKey(int key, bool boolean) { pressedkeys[key] = boolean; }
			inline void setLastMousePosition(glm::vec2 position) { lastMousePosition = position; }
			inline glm::vec2 getLastMousePosition() const { return lastMousePosition; }
			inline std::map<std::string, int>* getKeyBindings() { return &keyBindings; }
			inline void setKeyBinding(const std::string& key, int value) { keyBindings[key] = value; }
			inline std::pair<std::string, std::shared_ptr<Text>>* getCurrentEditedKeyBinding() { return &currentEditedKeyBinding; }
			inline void setCurrentEditedKeyBinding(std::pair<std::string, std::shared_ptr<Text>> value) { currentEditedKeyBinding = value; }
			bool resetCurrentEditedKeyBinding();
		private:
			std::pair<std::string, std::shared_ptr<Text>> currentEditedKeyBinding;
			std::map<std::string, int> keyBindings;
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
			bool pressedkeys[256];
	};
}
#endif