#ifndef applicationH
#define applicationH

#include <irrKlang/irrKlang.h>
#include <memory>

namespace Engine
{
	class CollisionManager;
	class ConfigurationManager;
	class InputManager;
	class MenuManager;
	class SpriteSheetManager;
	class Renderer;
	class GameStateManager;

	class Application
	{
		public:
			~Application();
			Application();
			Application(const Application &) = default;
			Application& operator=(const Application&) = default;
			Application(Application&&) = default;
			Application& operator=(Application&&) = default;

			void render();

			void savePlayerConfig();
			void loadPlayerConfig();

			inline irrklang::ISoundEngine* getSoundEngine() const { return m_soundEngine; }
			inline const std::unique_ptr<InputManager>& getInputManager() const { return m_inputManager; }
			inline const std::unique_ptr<MenuManager>& getMenuManager() const { return m_menuManager; }
			inline const std::unique_ptr<SpriteSheetManager>& getSpritesheetManager() const { return m_spriteSheetManager; }
			inline const std::unique_ptr<ConfigurationManager>& getConfigurationManager() const { return m_configurationManager; }
			inline const std::unique_ptr<CollisionManager>& getCollisionManager() const { return m_collisionManager; }
			inline const std::unique_ptr<Renderer>& getRenderer() const { return m_renderer; }
			inline const std::unique_ptr<GameStateManager>& getGameStateManager() const { return m_gameStateManager; }
		private:
			std::unique_ptr<ConfigurationManager> m_configurationManager{ std::make_unique<ConfigurationManager>() };
			std::unique_ptr<MenuManager> m_menuManager{ std::make_unique<MenuManager>() };
			std::unique_ptr<SpriteSheetManager> m_spriteSheetManager{ std::make_unique<SpriteSheetManager>() };
			std::unique_ptr<CollisionManager> m_collisionManager{ std::make_unique<CollisionManager>() };
			std::unique_ptr<InputManager> m_inputManager{ std::make_unique<InputManager>() };
			std::unique_ptr<GameStateManager> m_gameStateManager{ std::make_unique<GameStateManager>() };

			std::unique_ptr<Renderer> m_renderer{ std::make_unique<Renderer>() };

			irrklang::ISoundEngine* m_soundEngine{ irrklang::createIrrKlangDevice() };

			const float m_dt{ 1.0f / 60.0f };
	};
}
#endif