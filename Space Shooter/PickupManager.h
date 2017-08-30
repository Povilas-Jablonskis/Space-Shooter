#ifndef _pickupManagerH
#define _pickupManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "EffectManager.h"
#include "SpriteSheetManager.h"
#include "Pickup.h"

namespace Engine
{
	class PickupManager
	{
		public:
			void loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager>, std::shared_ptr<EffectManager>);
			std::shared_ptr<BaseGameObject> getPickup(std::string);
			std::shared_ptr<BaseGameObject> getRandomPickup();
		private:
			std::vector<std::pair<std::string, std::shared_ptr<BaseGameObject>>> pickups;
	};
}
#endif