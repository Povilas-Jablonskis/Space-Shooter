#ifndef _observerH
#define _observerH

#include <functional>
#include <vector>

#include "ObserverEvent.h"

namespace Engine
{
	class BaseGameObject;
	class Observer
	{
		public:
			virtual void onNotify(ObserverEvent) = 0;
			virtual void onNotify(ObserverEvent, BaseGameObject*) = 0;
	};
}
#endif