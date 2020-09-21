#ifndef gameStateManagerH
#define gameStateManagerH

namespace Engine
{
	enum class GameState
	{
		IN_MENU,
		STARTED,
		ENDED,
		IN_PAUSED_MENU
	};

	class GameStateManager
	{
		public:
			void setGameState(const GameState state) { m_lastGameState = getGameState(); m_gameState = state; }
			GameState getGameState() const { return m_gameState; }
			GameState getLastGameState() const { return m_lastGameState; }
		private:
			GameState m_lastGameState{ GameState::IN_MENU };
			GameState m_gameState{ GameState::IN_MENU };
	};
}
#endif