#ifndef _subjectH
#define _subjectH

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

#include "Observer.h"

namespace Engine
{
	class Subject
	{
		public:
			inline void addObserver(Observer* observer) { observers.push_back(observer); }
			void notify(ObserverEvent);
			void notify(ObserverEvent, BaseGameObject*);
		private:
			std::vector<Observer*> observers;
	};
}
#endif