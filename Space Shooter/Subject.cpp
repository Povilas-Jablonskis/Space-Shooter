#include "Subject.h"

namespace Engine
{
	void Subject::addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void Subject::removeObserver(Observer* observer)
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void Subject::notifyBase(ObserverEvent _event)
	{
		for (auto observer : observers)
		{
			observer->onNotifyBase(_event);
		}
	}

	void Subject::notifyCollision(ObserverEvent _event, BaseGameObject* _collider)
	{
		for (auto observer : observers)
		{
			observer->onNotifyCollision(_event, _collider);
		}
	}
}