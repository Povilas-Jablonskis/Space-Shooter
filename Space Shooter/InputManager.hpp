#ifndef inputManagerH
#define inputManagerH

#include <irrKlang/irrKlang.h>
#include "glm/vec2.hpp"
#include <Windows.h>
#include <algorithm>
#include <vector>
#include <memory>

namespace Engine
{
	class MenuManager;
	class SpriteSheetManager;
	class GameStateManager;

	typedef std::pair<int, bool> keyState;
	typedef std::pair<std::string, int> keyBinding;

	class InputManager
	{
		public:
			inline bool getLeftMouseState() const { return m_leftMouseClick; }
			inline bool getRightMouseState() const { return m_rightMouseClick; }
			inline void setLeftMouseState(bool boolean) { m_leftMouseClick = boolean; }
			inline void setRightMouseState(bool boolean) { m_rightMouseClick = boolean; }
			inline bool getLastLeftMouseState() const { return m_lastLeftMouseClick; }
			inline bool getLastRightMouseState() const { return m_lastRightMouseClick; }
			inline void setLastLeftMouseState(bool boolean) { m_lastLeftMouseClick = boolean; }
			inline void setLastRightMouseState(bool boolean) { m_lastRightMouseClick = boolean; }
			inline void setLastMousePosition(const glm::vec2& position) { m_lastMousePosition = position; }
			inline const glm::vec2& getLastMousePosition() const { return m_lastMousePosition; }
			bool getKey(char);
			bool getKey(const std::string&);
			void setKey(char, bool);
			bool getLastKey(char);
			bool getLastKey(const std::string&);
			void setLastKey(char, bool);
			inline std::vector<keyState>* getLastKeys() { return &m_lastKeyStates; }
			inline std::vector<keyState>* getKeys() { return &m_keyStates; }
			inline std::vector<keyBinding>* getKeyBindings() { return &m_keyBindings; }
			void setKeyBinding(keyBinding);
			inline const std::string& getCurrentlyEditedKeyBinding() const { return m_currentlyEditedKeyBinding; }
			inline void setCurrentlyEditedKeyBinding(const std::string& str) { m_currentlyEditedKeyBinding = str; }

			void keyboardInput(unsigned char, int, int, const std::unique_ptr<MenuManager>&, irrklang::ISoundEngine*, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<SpriteSheetManager>&);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void keyboardInputUp(unsigned char, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);

			static const std::string virtualKeyCodeToString(int);
		private:
			glm::vec2 m_lastMousePosition{ glm::vec2(0.0f, 0.0f) };
			bool m_lastLeftMouseClick{ false };
			bool m_leftMouseClick{ false };
			bool m_lastRightMouseClick{ false };
			bool m_rightMouseClick{ false };
			std::vector<keyState> m_keyStates;
			std::vector<keyState> m_lastKeyStates;
			std::vector<keyBinding> m_keyBindings;
			std::string m_currentlyEditedKeyBinding{""};
	};
}
#endif