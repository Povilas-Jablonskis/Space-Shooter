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
	class InputManager;
	class LevelManager;
	class GameStateManager;
	class SpriteSheetManager;

	class MenuManager
	{
		public:
			std::vector<std::shared_ptr<Menu>>* getMenus() { return &m_menus; }
			int getCharacterSelectionIndex() const { return m_characterSelectionIndex; }
			void setCharacterSelectionIndex(const int t_setCharacterSelectionIndex) { m_characterSelectionIndex = t_setCharacterSelectionIndex; }

			const std::shared_ptr<LevelManager>& getLevelManager() const { return m_levelManager; }

			void renderCurrentMenu(const std::shared_ptr<Renderer>&, float, const std::shared_ptr<InputManager>&);

			void loadPlayerModels(const std::shared_ptr<SpriteSheetManager>&);
			void initGameMenus(irrklang::ISoundEngine*, InputManager*, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<SpriteSheetManager>&);

			void escapeAction(irrklang::ISoundEngine*, InputManager*, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<SpriteSheetManager>&);
		private:
			void savePlayerConfig(irrklang::ISoundEngine*) const;
			void addMenu(const std::shared_ptr<Menu>& t_menu) { m_menus.push_back(t_menu); }
			const std::vector<std::shared_ptr<Animation>>& getPlayerModels() const { return m_playerModels; }

			std::shared_ptr<LevelManager> m_levelManager{ nullptr };

			std::vector<std::shared_ptr<Menu>> m_menus;
			int m_characterSelectionIndex{ 0 };
			std::vector<std::shared_ptr<Animation>> m_playerModels;
	};
}
#endif