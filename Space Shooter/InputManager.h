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
			bool GetLeftMouseState() const;
			bool GetRightMouseState() const;
			void SetLeftMouseState(bool);
			void SetRightMouseState(bool);
			bool GetLastLeftMouseState() const;
			bool GetLastRightMouseState() const;
			void SetLastLeftMouseState(bool);
			void SetLastRightMouseState(bool);
			bool GetKey(char) const;
			bool GetKey(int) const;
			void SetKey(char, bool);
			void SetKey(int, bool);
			void SetLastMousePosition(glm::vec2);
			glm::vec2 GetLastMousePosition() const;
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