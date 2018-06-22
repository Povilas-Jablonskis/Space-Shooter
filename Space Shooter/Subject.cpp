#include "Subject.h"
#include "BaseGameObject.h"

namespace Engine
{
	void Subject::addObserver(Observer* observer)
	{
		observers.push_back(std::move(observer));
	}

	void Subject::notify(ObserverEvent _event, std::map<std::string, BaseGameObject*> params)
	{
		for (auto observer : observers)
		{
			observer->onNotify(_event, params);
		}
	}
}