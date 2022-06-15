#ifndef uiManagerH
#define uiManagerH

#include <string>
#include <memory>
#include <vector>

class SpriteSheetManager;
class InputManager;
class Renderer;
class UIElementBase;

class UIManager
{
public:
	void renderUI(const std::shared_ptr<Renderer>&);
	void updateUI(int, const std::string&, int, float, const std::shared_ptr<InputManager>&, const std::shared_ptr<SpriteSheetManager>&);
private:
	std::vector<std::shared_ptr<UIElementBase>> m_playerLives;
	std::vector<std::shared_ptr<UIElementBase>> m_scoreBoard;
};
#endif