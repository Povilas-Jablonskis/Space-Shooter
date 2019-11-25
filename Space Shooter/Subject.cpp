#include "Subject.hpp"
#include "Observer.hpp"

namespace Engine
{
	void Subject::notify(ObserverEvent event)
	{
		auto observers = getObservers();

		for (auto observer : observers)
		{
			observer->onNotify(event);
		}
	}

	void Subject::notify(ObserverEvent event, BaseGameObject* obj)
	{
		auto observers = getObservers();

		for (auto observer : observers)
		{
			observer->onNotify(event, obj);
		}
	}
}