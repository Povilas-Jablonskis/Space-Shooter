#include "Subject.h"
#include "BaseGameObject.h"

namespace Engine
{
	void Subject::addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void Subject::notify(ObserverEvent _event, std::vector<std::pair<std::string, BaseGameObject*>> params)
	{
		for (auto observer : observers)
		{
			observer->onNotify(_event, params);
		}
	}
}