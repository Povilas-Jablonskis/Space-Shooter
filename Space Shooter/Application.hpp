#ifndef applicationH
#define applicationH

#include <irrKlang/irrKlang.h>
#include <memory>

class InputManager;
class MenuManager;
class SpriteSheetManager;
class Renderer;
class GameStateManager;

class Application
{
public:
	Application();

	void render() const;

	void loadPlayerConfig() const;

	irrklang::ISoundEngine* getSoundEngine() const { return m_soundEngine; }
	const std::shared_ptr<InputManager>& getInputManager() const { return m_inputManager; }
	const std::shared_ptr<MenuManager>& getMenuManager() const { return m_menuManager; }
	const std::shared_ptr<SpriteSheetManager>& getSpritesheetManager() const { return m_spriteSheetManager; }
	const std::shared_ptr<Renderer>& getRenderer() const { return m_renderer; }
	const std::shared_ptr<GameStateManager>& getGameStateManager() const { return m_gameStateManager; }
private:
	std::shared_ptr<MenuManager> m_menuManager{ std::make_shared<MenuManager>() };
	std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{ std::make_shared<SpriteSheetManager>() };
	std::shared_ptr<InputManager> m_inputManager{ std::make_shared<InputManager>() };
	std::shared_ptr<GameStateManager> m_gameStateManager{ std::make_shared<GameStateManager>() };

	std::shared_ptr<Renderer> m_renderer{ std::make_shared<Renderer>() };

	irrklang::ISoundEngine* m_soundEngine{ irrklang::createIrrKlangDevice() };

	const float m_dt{ 1.0f / 60.0f };
};
#endif