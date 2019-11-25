#ifndef levelManagerH
#define levelManagerH

#include <irrKlang.h>
#include <memory>
#include <vector>

namespace Engine
{
	class CollisionManager;
	class SpriteSheetManager;
	class Level;
	class GameStateManager;
	class InputManager;
	class Renderer;
	class ConfigurationManager;

	class LevelManager
	{
		public:
			void initGameLevels(const std::unique_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*, int);
			inline void newLevel() { m_levels.pop_back(); }
			inline const std::shared_ptr<Level>& getCurrentLevel() const { return m_levels.back(); }
			void renderCurrentLevel(float, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<InputManager>&, const std::unique_ptr<CollisionManager>&, const std::unique_ptr<Renderer>&, const std::unique_ptr<ConfigurationManager>&, const std::unique_ptr<SpriteSheetManager>&);
		private:
			std::vector<std::shared_ptr<Level>> m_levels;
	};
}
#endif