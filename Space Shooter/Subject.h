#ifndef _subjectH
#define _subjectH

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

#include "Observer.h"

namespace Engine
{
	class BaseGameObject;
	class Subject
	{
		public:
			void addObserver(Observer*);
			void notify(ObserverEvent, std::vector<std::pair<std::string, BaseGameObject*>>);
		private:
			std::vector<Observer*> observers;
	};
}
#endif