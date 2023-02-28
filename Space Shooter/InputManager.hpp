#ifndef inputManagerH
#define inputManagerH

#include "glm/vec2.hpp"
#include <memory>
#include <string>
#include <unordered_map>

#include "KeyBinding.hpp"

class InputManager
{
public:
	[[nodiscard]] bool isLeftMouseClicked() const;
	void setLeftMouseClicked(bool);
	void setRightMouseClicked(bool);
	[[nodiscard]] bool isRightMouseClicked() const;
	[[nodiscard]] bool isLastLeftMouseStateClicked() const;
	void setLastLeftMouseClicked(bool);
	[[nodiscard]] bool isLastRightMouseStateClicked() const;
	void setLastRightMouseClicked(bool);
	[[nodiscard]] const glm::vec2& getLastMousePosition() const;
	void setLastMousePosition(const glm::vec2&);
	[[nodiscard]] bool isKeyActive(short) const;
	[[nodiscard]] bool isKeyActive(const std::string&) const;
	void setKeyState(short, bool);
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

	void checkInteraction(const std::shared_ptr<Text>&) const;

private:
	template <typename Iterator, typename Predicate, typename Op>
	static void sFor_each_if(Iterator first, Iterator last, Predicate p, Op op)
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
