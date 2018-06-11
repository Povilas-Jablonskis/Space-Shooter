#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"
#include "vec4.hpp"
#include <memory>
#include <iostream>

#include "Text.h"
#include "Player.h"

namespace Engine
{
	typedef std::pair<std::string, int> keybinding;
	typedef std::pair<std::vector<keybinding>::iterator, std::shared_ptr<Text>> currentEditedKeyBinding;

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
			inline std::vector<keybinding>* getKeyBindings() { return &keyBindings; }
			void setKeyBinding(const std::string&, int);
			int getKeyBinding(const std::string&);
			inline currentEditedKeyBinding* getCurrentEditedKeyBinding() { return &theCurrentEditedKeyBinding; }
			inline void setCurrentEditedKeyBinding(currentEditedKeyBinding value) { theCurrentEditedKeyBinding = value; }
			bool resetCurrentEditedKeyBinding();
		private:
			const int pressedKeyCount = 256;
			currentEditedKeyBinding theCurrentEditedKeyBinding;
			std::vector<keybinding> keyBindings;
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
			bool* pressedkeys;
	};
}
#endif