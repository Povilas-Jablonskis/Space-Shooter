#ifndef _observerH
#define _observerH

#include <functional>

#include "ObserverEvent.h"

namespace Engine
{
	class BaseGameObject;
	class Observer
	{
		public:
			Observer();
			std::function<void(ObserverEvent)> onNotifyBase;
			std::function<void(ObserverEvent, BaseGameObject*)> onNotifyCollision;
	};
}
#endif