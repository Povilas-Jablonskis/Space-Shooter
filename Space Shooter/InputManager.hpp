#ifndef inputManagerH
#define inputManagerH

#include "glm/vec2.hpp"
#include <memory>
#include <unordered_map>

#include "BindableAction.hpp"
#include "Text.hpp"

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
	void changeKeyState(short, bool);
	std::unordered_map<short, bool>& getKeysStates();

	void keyboardInput(unsigned char);
	void motionFunc(int, int);
	void processMouseClick(int, int, int, int);
	void keyboardInputUp(unsigned char, int, int);
	void specialKeyInput(int, int, int);
	void specialKeyInputUp(int, int, int);

	void clearEverything();

	void checkInteraction(const std::shared_ptr<Text>&) const;

	[[nodiscard]] short getKeybind(BindableAction) const;
	void setKeybindCharacter(BindableAction, short);
	[[nodiscard]] bool keybindingsContainKey(const short) const;
	void loadKeybinds();
	void saveKeybinds() const;
	[[nodiscard]] std::string virtualKeyCodeToString(int) const;

private:
	glm::vec2 m_lastMousePosition{};
	bool m_lastLeftMouseClick{};
	bool m_leftMouseClick{};
	bool m_lastRightMouseClick{};
	bool m_rightMouseClick{};
	std::unordered_map<short, bool> m_keysStates;
	std::unordered_map<BindableAction, short> m_keybinds;
};
#endif
