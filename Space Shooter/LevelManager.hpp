#ifndef levelManagerH
#define levelManagerH

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include <queue>

#include "Level.hpp"

namespace Engine
{
	class CollisionManager;
	class SpriteSheetManager;
	class GameStateManager;
	class InputManager;
	class Renderer;
	class ConfigurationManager;
	class Player;

	class LevelManager
	{
		public:
			LevelManager(const std::unique_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*, int);			
			inline const std::unique_ptr<Level>& getCurrentLevel() const { return m_levels.front(); }
			void renderCurrentLevel(float, const std::unique_ptr<GameStateManager>&, const std::unique_ptr<InputManager>&, const std::unique_ptr<CollisionManager>&, const std::unique_ptr<Renderer>&, const std::unique_ptr<ConfigurationManager>&, const std::unique_ptr<SpriteSheetManager>&);
		private:
			std::shared_ptr<Player> m_player{ nullptr };

			float m_currentTime{ static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) };
			float m_accumulator{ 0.0f };
			std::queue<std::unique_ptr<Level>> m_levels;
	};
}
#endif