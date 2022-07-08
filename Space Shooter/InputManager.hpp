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
	bool getLeftMouseState() const;
	void setLeftMouseState(bool);
	void setRightMouseState(bool);
	bool getRightMouseState() const;
	bool getLastLeftMouseState() const;
	void setLastLeftMouseState(bool);
	bool getLastRightMouseState() const;
	void setLastRightMouseState(bool);
	const glm::vec2& getLastMousePosition() const;
	void setLastMousePosition(const glm::vec2&);
	bool getKey(short) const;
	bool getKey(const std::string&) const;
	void setKey(short, bool);
	std::unordered_map<short, bool>& getKeys();
	std::vector<std::shared_ptr<KeyBinding>>& getKeyBindings();
	void addKeyBinding(const std::shared_ptr<KeyBinding>&);
	std::shared_ptr<KeyBinding> getCurrentlyEditedKeyBinding() const;

	void keyboardInput(unsigned char);
	void motionFunc(int, int);
	void processMouseClick(int, int, int, int);
	void keyboardInputUp(unsigned char, int, int);
	void specialKeyInput(int, int, int);
	void specialKeyInputUp(int, int, int);

	void clearEverything();
private:
	template<typename Iter, typename Pred, typename Op>
	void for_each_if(Iter first, Iter last, Pred p, Op op) {
		while (first != last) {
			if (p(*first)) op(*first);
			++first;
		}
	}

	glm::vec2 m_lastMousePosition{};
	bool m_lastLeftMouseClick{};
	bool m_leftMouseClick{};
	bool m_lastRightMouseClick{};
	bool m_rightMouseClick{};
	std::unordered_map<short, bool> m_keyStates;
	std::vector<std::shared_ptr<KeyBinding>> m_keyBindings;
};
#endif