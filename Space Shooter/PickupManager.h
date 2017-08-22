#ifndef _pickupManagerH
#define _pickupManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "SpriteSheetManager.h"
#include "Pickup.h"

namespace Engine
{
	class Player;
	class PickupManager
	{
		public:
			void loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager>, std::map<std::string, std::function<void(Player*)>>);
			std::shared_ptr<Pickup> getPickup(std::string);
		private:
			std::map<std::string, std::shared_ptr<Pickup>> pickups;
	};
}
#endif