#ifndef _subjectH
#define _subjectH

#include "Observer.h"

#include <vector>
#include <memory>

namespace Engine
{
	class Subject
	{
		public:
			Subject::Subject()
			{
				observer = std::shared_ptr<Observer>();
			}
			void notify(const std::string& str)
			{
				observer->onNotify(str);
			}

		private:
			std::shared_ptr<Observer> observer;
	};
}
#endif