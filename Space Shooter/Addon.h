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
			inline void setNeedsToBeRemoved(bool boolean) { needsToBeRemoved = boolean; };
			inline bool getNeedsToBeRemoved() const { return needsToBeRemoved; }
			inline void setPositionOffset(glm::vec2 offset) { positionOffset = offset; };
			inline glm::vec2 getPositionOffset() const { return positionOffset; }
			std::function<void()> onDeath;
		private:
			bool needsToBeRemoved;
			glm::vec2 positionOffset;
	};
}
#endif