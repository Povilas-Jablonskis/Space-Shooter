#include "Observer.h"
#include "BaseGameObject.h"

namespace Engine
{
	Observer::Observer()
	{
		onNotify = [](ObserverEvent _event, std::vector<std::pair<std::string, BaseGameObject*>> params)
		{

		};
	};

	BaseGameObject* Observer::getParameter(std::vector<std::pair<std::string, BaseGameObject*>> v, std::string index)
	{
		for (auto item : v)
		{
			if (item.first == index)
				return item.second;
		}
		
		return nullptr;
	}
}