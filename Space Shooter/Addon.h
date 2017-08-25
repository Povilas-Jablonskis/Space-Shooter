#ifndef _addonH
#define _addonH

#include "RenderObject.h"

namespace Engine
{
	class Addon : public RenderObject
	{
		public:
			Addon(float, float, glm::vec2);
			bool update(float, glm::vec2);
			inline void setNeedsToBeDeleted(bool boolean) { needsToBeDeleted = boolean; };
			inline bool getNeedsToBeDeleted() const { return needsToBeDeleted; }
		private:
			bool needsToBeDeleted;
			glm::vec2 positionOffset;
	};
}
#endif