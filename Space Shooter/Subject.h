#ifndef _subjectH
#define _subjectH

#include <vector>
#include <map>
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
			void removeObserver(Observer*);
			void notify(ObserverEvent, std::map<std::string, BaseGameObject*>);
		private:
			std::vector<Observer*> observers;
	};
}
#endif