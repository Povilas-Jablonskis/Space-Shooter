#include "Subject.h"
#include "BaseGameObject.h"

namespace Engine
{
	void Subject::notify(ObserverEvent _event)
	{
		for (auto observer : observers)
		{
			observer->onNotify(_event);
		}
	}

	void Subject::notify(ObserverEvent _event, BaseGameObject* obj)
	{
		for (auto observer : observers)
		{
			observer->onNotify(_event, obj);
		}
	}
}