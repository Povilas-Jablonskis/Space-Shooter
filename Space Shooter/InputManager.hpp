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
	void setLeftMouseClicked(const bool leftMouseClick);
	void setRightMouseClicked(const bool rightMouseClick);
	[[nodiscard]] bool isRightMouseClicked() const;
	[[nodiscard]] bool isLastLeftMouseStateClicked() const;
	void setLastLeftMouseClicked(const bool lastLeftMouseClick);
	[[nodiscard]] bool isLastRightMouseStateClicked() const;
	void setLastRightMouseClicked(const bool lastRightMouseClick);
	[[nodiscard]] const glm::vec2& getLastMousePosition() const;
	void setLastMousePosition(const glm::vec2& lastMousePosition);
	[[nodiscard]] bool isKeyActive(const short key) const;
	void changeKeyState(const short key, bool boolean);
	std::unordered_map<short, bool>& getKeysStates();

	void keyboardInput(const unsigned char c);
	void motionFunc(const int x, const int y);
	void processMouseClick(const int button, const int state, const int x, const int y);
	void keyboardInputUp(const unsigned char c, int, int);
	void specialKeyInput(const int key, int, int);
	void specialKeyInputUp(const int key, int, int);

	void clearEverything();

	void checkInteraction(const std::shared_ptr<Text>& text) const;

	[[nodiscard]] short getKeybind(const BindableAction bindableAction) const;
	void setKeybindCharacter(BindableAction key, short value);
	[[nodiscard]] bool keybindingsContainKey(const short keyBind) const;
	void loadKeybinds();
	void saveKeybinds() const;
	[[nodiscard]] std::string virtualKeyCodeToString(const int virtualKey) const;

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
