#ifndef _inputManagerH
#define _inputManagerH

#include "vec2.hpp"

namespace Engine
{
	class InputManager
	{
		public:
			~InputManager();
			InputManager();
			bool getLeftMouseState() const;
			bool getRightMouseState() const;
			void setLeftMouseState(bool);
			void setRightMouseState(bool);
			bool getLastLeftMouseState() const;
			bool getLastRightMouseState() const;
			void setLastLeftMouseState(bool);
			void setLastRightMouseState(bool);
			bool getKey(char) const;
			bool getKey(int) const;
			void setKey(char, bool);
			void setKey(int, bool);
			void setLastMousePosition(glm::vec2);
			glm::vec2 getLastMousePosition() const;
		private:
			glm::vec2 lastMousePosition;
			bool lastLeftMouseClick;
			bool leftMouseClick;
			bool lastRightMouseClick;
			bool rightMouseClick;
			bool pressedkeys[256];
	};
}
#endif