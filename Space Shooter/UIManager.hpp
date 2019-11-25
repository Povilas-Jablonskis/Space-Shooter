#ifndef uiManagerH
#define uiManagerH

#include <string>
#include <memory>
#include <vector>

namespace Engine
{
	class Text;
	class SpriteSheetManager;
	class GameStateManager;
	class InputManager;
	class Renderer;
	class ConfigurationManager;
	class UIElementBase;

	typedef std::pair<std::string, std::shared_ptr<Text>> uiPlayerElement;

	class UIManager
	{
		public:
			void updatePlayerLives(const std::unique_ptr<SpriteSheetManager>&, const std::string&, int);
			void updatePlayerScore(const std::unique_ptr<SpriteSheetManager>&, int);
			void render(float, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<InputManager>&, const std::unique_ptr<Renderer>&, const std::unique_ptr<ConfigurationManager>&);
		private:

			std::vector<std::shared_ptr<UIElementBase>> m_playerLives;
			std::vector<std::shared_ptr<UIElementBase>> m_scoreBoard;

			std::vector<uiPlayerElement> m_notifications;
	};
}
#endif