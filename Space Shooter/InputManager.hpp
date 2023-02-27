#ifndef inputManagerH
#define inputManagerH

#include "glm/vec2.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class KeyBinding;

class InputManager
{
public:
	[[nodiscard]] bool getLeftMouseState() const;
	void setLeftMouseState(bool);
	void setRightMouseState(bool);
	[[nodiscard]] bool getRightMouseState() const;
	[[nodiscard]] bool getLastLeftMouseState() const;
	void setLastLeftMouseState(bool);
	[[nodiscard]] bool getLastRightMouseState() const;
	void setLastRightMouseState(bool);
	[[nodiscard]] const glm::vec2& getLastMousePosition() const;
	void setLastMousePosition(const glm::vec2&);
	[[nodiscard]] bool getKey(short) const;
	[[nodiscard]] bool getKey(const std::string&) const;
	void setKey(short, bool);
	std::unordered_map<short, bool>& getKeys();
	std::vector<std::shared_ptr<KeyBinding>>& getKeyBindings();
	void addKeyBinding(const std::shared_ptr<KeyBinding>&);
	[[nodiscard]] std::shared_ptr<KeyBinding> getCurrentlyEditedKeyBinding() const;

	void keyboardInput(char);
	void motionFunc(int, int);
	void processMouseClick(int, int, int, int);
	void keyboardInputUp(char, int, int);
	void specialKeyInput(int, int, int);
	void specialKeyInputUp(int, int, int);

	void clearEverything();

private:
	template <typename Iterator, typename Predicate, typename Op>
	static void for_each_if(Iterator first, Iterator last, Predicate p, Op op)
	{
		while (first != last)
		{
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
