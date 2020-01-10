#ifndef menuManagerH
#define menuManagerH

#include <irrKlang/irrKlang.h>
#include <vector>
#include <memory>

namespace Engine
{
	class Menu;
	class Animation;
	class Renderer;
	class ConfigurationManager;
	class InputManager;
	class LevelManager;
	class GameStateManager;
	class SpriteSheetManager;

	class MenuManager
	{
		public:
			inline std::vector<std::shared_ptr<Menu>>* getMenus() { return &m_menus; }
			inline int getCharacterSelectionIndex() const { return m_characterSelectionIndex; }
			inline void setCharacterSelectionIndex(int t_setCharacterSelectionIndex) { m_characterSelectionIndex = t_setCharacterSelectionIndex; }

			inline const std::unique_ptr<LevelManager>& getLevelManager() const { return m_levelManager; }

			void renderCurrentMenu(const std::unique_ptr<Renderer>&, float, const std::unique_ptr<ConfigurationManager>&, const std::unique_ptr<InputManager>&);

			void loadPlayerModels(const std::unique_ptr<SpriteSheetManager>&);
			void initGameMenus(irrklang::ISoundEngine*, InputManager*, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<SpriteSheetManager>&);

			void escapeAction(irrklang::ISoundEngine*, InputManager*, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<SpriteSheetManager>&);
		private:
			inline void addMenu(const std::shared_ptr<Menu>& t_menu) { m_menus.push_back(t_menu); }
			inline const std::vector<std::shared_ptr<Animation>>& getPlayerModels() const { return m_playerModels; }

			std::unique_ptr<LevelManager> m_levelManager{ nullptr };

			std::vector<std::shared_ptr<Menu>> m_menus;
			int m_characterSelectionIndex{ 0 };
			std::vector<std::shared_ptr<Animation>> m_playerModels;
	};
}
#endif