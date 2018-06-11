#ifndef _pickupManagerH
#define _pickupManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include <irrKlang.h>

#include "Pickup.h"
#include "SpriteSheetManager.h"
#include "EffectManager.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Pickup>> pickup;

	class PickupManager
	{
		public:
			void loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager>, std::shared_ptr<EffectManager>);
			std::shared_ptr<Pickup> getPickup(std::string);
			std::shared_ptr<Pickup> getRandomPickup();
		private:
			std::vector<pickup> pickups;
	};
}
#endif