#include "Observer.h"

namespace Engine
{
	Observer::Observer()
	{
		onNotifyBase = [](ObserverEvent _event)
		{

		};

		onNotifyCollision = [](ObserverEvent _event, BaseGameObject* _obj)
		{

		};
	};
}