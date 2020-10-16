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

	class UIManager
	{
		public:
			void addNotification(const std::shared_ptr<Text>& text) { m_notifications.push_back(text); };
			void renderUI(const std::shared_ptr<Renderer>&);
			void updateUI(int, const std::string&, int, float, const std::shared_ptr<InputManager>&, const std::shared_ptr<ConfigurationManager>&, const std::shared_ptr<SpriteSheetManager>&);
		private:
			std::vector<std::shared_ptr<UIElementBase>> m_playerLives;
			std::vector<std::shared_ptr<UIElementBase>> m_scoreBoard;

			std::vector<std::shared_ptr<Text>> m_notifications;
	};
}
#endif