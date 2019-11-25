#ifndef observerH
#define observerH

namespace Engine
{
	enum ObserverEvent
	{
		PLAYER_DIED,
		OBJECT_DESTROYED
	};

	class BaseGameObject;

	class Observer
	{
		public:
			virtual void onNotify(ObserverEvent) = 0;
			virtual void onNotify(ObserverEvent, BaseGameObject*) = 0;
	};
}
#endif