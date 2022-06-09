#ifndef gameStateManagerH
#define gameStateManagerH

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
	void setGameState(const GameState state) { m_gameState = state; }
	GameState getGameState() const { return m_gameState; }
private:
	GameState m_gameState{ GameState::IN_MENU };
};
#endif