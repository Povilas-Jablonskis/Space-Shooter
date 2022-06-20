#ifndef inputManagerH
#define inputManagerH

#include <irrKlang/irrKlang.h>
#include "glm/vec2.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class KeyBinding;

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
	std::unordered_map<short, bool>* getKeys() { return &m_keyStates; }
	std::vector<std::shared_ptr<KeyBinding>>* getKeyBindings() { return &m_keyBindings; }
	void addKeyBinding(const std::shared_ptr<KeyBinding>& key_binding) { m_keyBindings.push_back(key_binding); }
	std::shared_ptr<KeyBinding> getCurrentlyEditedKeyBinding() const { return m_currentlyEditedKeyBinding; }
	void setCurrentlyEditedKeyBinding(const std::shared_ptr<KeyBinding>& kb) { m_currentlyEditedKeyBinding = kb; }

	void keyboardInput(unsigned char);
	void motionFunc(int, int);
	void processMouseClick(int, int, int, int);
	void keyboardInputUp(unsigned char, int, int);
	void specialKeyInput(int, int, int);
	void specialKeyInputUp(int, int, int);

	void clearEverything();

	static std::string virtualKeyCodeToString(int);
private:
	glm::vec2 m_lastMousePosition{};
	bool m_lastLeftMouseClick{};
	bool m_leftMouseClick{};
	bool m_lastRightMouseClick{};
	bool m_rightMouseClick{};
	std::unordered_map<short, bool> m_keyStates;
	std::vector<std::shared_ptr<KeyBinding>> m_keyBindings;
	std::shared_ptr<KeyBinding> m_currentlyEditedKeyBinding{ nullptr };
};
#endif