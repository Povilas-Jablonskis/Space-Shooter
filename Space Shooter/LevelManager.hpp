#ifndef levelManagerH
#define levelManagerH

#include <glew/glew.h>
#include <freeglut/freeglut.h>
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
			LevelManager(const std::shared_ptr<SpriteSheetManager>&, irrklang::ISoundEngine*, int);
			const std::shared_ptr<Level>& getCurrentLevel() const { return m_levels.front(); }
			void renderCurrentLevel(float, const std::shared_ptr<GameStateManager>&, const std::shared_ptr<InputManager>&, const std::shared_ptr<CollisionManager>&, const std::shared_ptr<Renderer>&, const std::shared_ptr<ConfigurationManager>&, const std::shared_ptr<SpriteSheetManager>&);
		private:
			std::shared_ptr<Player> m_player{ nullptr };

			float m_currentTime{ static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) };
			float m_accumulator{ 0.0f };
			std::queue<std::shared_ptr<Level>> m_levels;
	};
}
#endif