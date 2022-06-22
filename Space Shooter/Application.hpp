#ifndef applicationH
#define applicationH

#include "ResourceAllocator.hpp"

#include <irrKlang/irrKlang.h>
#include <memory>

class SceneStateMachine;
class InputManager;
class SpriteSheetManager;
class Renderer;
class Texture;

class Application
{
public:
	Application();

	void render() const;

	void loadPlayerConfig() const;

	irrklang::ISoundEngine* getSoundEngine() const { return m_soundEngine; }
	const std::shared_ptr<InputManager>& getInputManager() const { return m_inputManager; }
	const std::shared_ptr<SceneStateMachine>& getSceneManager() const { return m_sceneManager; }
	const std::shared_ptr<SpriteSheetManager>& getSpritesheetManager() const { return m_spriteSheetManager; }
	const std::shared_ptr<Renderer>& getRenderer() const { return m_renderer; }
private:
	ResourceAllocator<Texture> m_textureAllocator{};
	std::shared_ptr<SceneStateMachine> m_sceneManager{ std::make_shared<SceneStateMachine>() };
	std::shared_ptr<SpriteSheetManager> m_spriteSheetManager{ std::make_shared<SpriteSheetManager>() };
	std::shared_ptr<InputManager> m_inputManager{ std::make_shared<InputManager>() };

	std::shared_ptr<Renderer> m_renderer{ std::make_shared<Renderer>() };

	irrklang::ISoundEngine* m_soundEngine{ irrklang::createIrrKlangDevice() };

	const float m_dt{ 1.0f / 60.0f };
};
#endif