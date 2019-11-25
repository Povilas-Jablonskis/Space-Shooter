#ifndef gameStateManagerH
#define gameStateManagerH

namespace Engine
{
	enum GameState
	{
		IN_MENU,
		STARTED,
		ENDED,
		IN_PAUSED_MENU
	};

	class GameStateManager
	{
		public:
			inline void setGameState(GameState state) { m_lastGameState = getGameState(); m_gameState = state; }
			inline GameState getGameState() const { return m_gameState; }
			inline GameState getLastGameState() const { return m_lastGameState; }
		private:
			GameState m_lastGameState{ GameState::IN_MENU };
			GameState m_gameState{ GameState::IN_MENU };
	};
}
#endif