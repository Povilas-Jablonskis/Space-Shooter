#ifndef inputManagerH
#define inputManagerH

#include <irrKlang/irrKlang.h>
#include "glm/vec2.hpp"
#include <vector>
#include <memory>
#include <string>

namespace Engine
{
	class MenuManager;
	class SpriteSheetManager;
	class GameStateManager;
	class KeyBinding;
	
	typedef std::pair<short, bool> keyState;

	class InputManager
	{
		public:
			bool getLeftMouseState() const { return m_leftMouseClick; }
			bool getRightMouseState() const { return m_rightMouseClick; }
			void setLeftMouseState(const bool boolean) { m_leftMouseClick = boolean; }
			void setRightMouseState(const bool boolean) { m_rightMouseClick = boolean; }
			bool getLastLeftMouseState() const { return m_lastLeftMouseClick; }
			bool getLastRightMouseState() const { return m_lastRightMouseClick; }
			void setLastLeftMouseState(const bool boolean) { m_lastLeftMouseClick = boolean; }
			void setLastRightMouseState(const bool boolean) { m_lastRightMouseClick = boolean; }
			void setLastMousePosition(const glm::vec2& position) { m_lastMousePosition = position; }
			const glm::vec2& getLastMousePosition() const { return m_lastMousePosition; }
			bool getKey(const short);
			bool getKey(const std::string&);
			void setKey(const short, bool);
			bool getLastKey(const short);
			bool getLastKey(const std::string&);
			void setLastKey(const short, bool);
			std::vector<keyState>* getLastKeys() { return &m_lastKeyStates; }
			std::vector<keyState>* getKeys() { return &m_keyStates; }
			std::vector<std::shared_ptr<KeyBinding>>* getKeyBindings() { return &m_keyBindings; }
			void addKeyBinding(const std::shared_ptr<KeyBinding>& key_binding) { m_keyBindings.push_back(key_binding); }
			std::shared_ptr<KeyBinding> getCurrentlyEditedKeyBinding() const { return m_currentlyEditedKeyBinding; }
			void setCurrentlyEditedKeyBinding(const std::shared_ptr<KeyBinding>& kb) { m_currentlyEditedKeyBinding = kb; }

			void keyboardInput(unsigned char, int, int, const std::shared_ptr<MenuManager>&, irrklang::ISoundEngine*, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<SpriteSheetManager>&);
			void motionFunc(int, int);
			void processMouseClick(int, int, int, int);
			void keyboardInputUp(unsigned char, int, int);
			void specialKeyInput(int, int, int);
			void specialKeyInputUp(int, int, int);

			static std::string virtualKeyCodeToString(int);
		private:
			glm::vec2 m_lastMousePosition{ glm::vec2(0.0f, 0.0f) };
			bool m_lastLeftMouseClick{ false };
			bool m_leftMouseClick{ false };
			bool m_lastRightMouseClick{ false };
			bool m_rightMouseClick{ false };
			std::vector<keyState> m_keyStates;
			std::vector<keyState> m_lastKeyStates;
			std::vector<std::shared_ptr<KeyBinding>> m_keyBindings;
			std::shared_ptr<KeyBinding> m_currentlyEditedKeyBinding = nullptr;
	};
}
#endif