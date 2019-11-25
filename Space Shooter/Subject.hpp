#ifndef subjectH
#define subjectH

#include <vector>

namespace Engine
{
	enum ObserverEvent;
	class BaseGameObject;
	class Observer;

	class Subject
	{
		public:
			inline void addObserver(Observer* observer) { m_observers.push_back(observer); }
			void notify(ObserverEvent);
			void notify(ObserverEvent, BaseGameObject*);
		private:
			const std::vector<Observer*>& getObservers() const { return m_observers; }
			std::vector<Observer*> m_observers;
	};
}
#endif