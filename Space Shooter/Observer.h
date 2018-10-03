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
			Observer();
			std::function<void(ObserverEvent, std::vector<std::pair<std::string, BaseGameObject*>>)> onNotify;
			BaseGameObject* getParameter(std::vector<std::pair<std::string, BaseGameObject*>>, std::string);
	};
}
#endif