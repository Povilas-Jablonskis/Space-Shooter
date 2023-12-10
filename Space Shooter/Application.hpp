#ifndef applicationH
#define applicationH

#include "SpriteSheet.hpp"
#include "Configs.hpp"
#include "InputManager.hpp"
#include "SceneStateMachine.hpp"
#include "SharedContext.hpp"

#include <irrKlang/irrKlang.h>

class Application
{
public:
	Application();
	Application(const Application&) = default;
	Application(Application&&) = default;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;
	~Application();

	void loadConfig();

	void render() const;

	InputManager& getInputManager();

private:
	SharedContext m_context;
	SpriteSheet m_spriteSheet{Configs::SPRITE_SHEETS_PATH};
	SceneStateMachine m_sceneManager;
	InputManager m_inputManager;
	Font m_font{"assets/Fonts/kenvector_future_thin.ttf"};

	Renderer m_renderer;

	irrklang::ISoundEngine* m_soundEngine{irrklang::createIrrKlangDevice()};

	const float m_dt{1.0f / 60.0f};
};
#endif
