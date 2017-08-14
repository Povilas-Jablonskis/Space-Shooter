#ifndef _subjectH
#define _subjectH

#include <map>
#include <memory>
#include <functional>

#include "ObserverEvent.h"

namespace Engine
{
	class Subject
	{
		public:
			void addObserver(ObserverEvent, std::function<void()>);
			void removeObserver(ObserverEvent);
		protected:
			void notify(ObserverEvent);
		private:
			std::map<ObserverEvent, std::function<void()>> observers;
	};
}
#endif