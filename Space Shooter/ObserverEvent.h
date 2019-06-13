#ifndef _observerEventH
#define _observerEventH

namespace Engine
{
	enum ObserverEvent
	{
		SCORE_CHANGED,
		LIVES_CHANGED,
		PLAYER_DIED,
		OBJECT_DESTROYED,
		GAMESTATE_CHANGED
	};
}
#endif