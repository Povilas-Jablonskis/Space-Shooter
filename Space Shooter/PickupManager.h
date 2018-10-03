#ifndef _pickupManagerH
#define _pickupManagerH

#include <rapidxml.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include <irrKlang.h>

#include "BaseGameObject.h"
#include "SpriteSheetManager.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<BaseGameObject>> pickup;

	class PickupManager
	{
		public:
			void loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager>);
			std::shared_ptr<BaseGameObject> getPickup(std::string);
			std::shared_ptr<BaseGameObject> getRandomPickup();
		private:
			std::vector<pickup> pickups;
	};
}
#endif