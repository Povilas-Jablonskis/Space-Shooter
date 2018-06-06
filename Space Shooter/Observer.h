#ifndef _observerH
#define _observerH

#include <functional>
#include <map>

#include "ObserverEvent.h"

namespace Engine
{
	class BaseGameObject;
	class Observer
	{
		public:
			Observer();
			std::function<void(ObserverEvent, std::map<std::string, BaseGameObject*>)> onNotify;
	};
}
#endif