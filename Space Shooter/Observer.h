#ifndef _observerH
#define _observerH

#include <string>

namespace Engine
{
	class Observer
	{
		public:
			void onNotify(const std::string& str)
			{
				#if _DEBUG
					std::cout << "notified!" << std::endl;
				#endif
			};
	};
}
#endif