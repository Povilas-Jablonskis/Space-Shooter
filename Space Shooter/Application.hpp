#ifndef applicationH
#define applicationH

#include "SpriteSheet.hpp"
#include "FileConstants.hpp"
#include "SharedContext.hpp"
#include "SceneStateMachine.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Font.hpp"

#include <irrKlang/irrKlang.h>

class Application
{
public:
	Application();

	void loadPlayerConfig() const;

	void render() const;

	InputManager& getInputManager();

private:
	SharedContext m_context;
	SpriteSheet m_spriteSheet{FileConstants::SPRITE_SHEETS_PATH};
	SceneStateMachine m_sceneManager;
	InputManager m_inputManager;
	Font m_font{"assets/Fonts/kenvector_future_thin.ttf"};

	Renderer m_renderer;

	irrklang::ISoundEngine* m_soundEngine{irrklang::createIrrKlangDevice()};

	const float m_dt{1.0f / 60.0f};
};
#endif
