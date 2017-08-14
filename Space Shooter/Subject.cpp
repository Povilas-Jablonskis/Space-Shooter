#include "Subject.h"

namespace Engine
{
	void Subject::addObserver(ObserverEvent _event, std::function<void()> _func)
	{
		if (observers.find(_event) == observers.end())
			observers.insert(std::pair<ObserverEvent, std::function<void()>>(_event, _func));
	}

	void Subject::removeObserver(ObserverEvent _event)
	{
		auto observer = observers.find(_event);

		if (observer != observers.end())
			observers.erase(observer);
	}

	void Subject::notify(ObserverEvent _event)
	{
		auto observer = observers.find(_event);

		if (observer != observers.end())
			observer->second();
	}
}