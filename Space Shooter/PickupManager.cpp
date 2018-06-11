#include "PickupManager.h"
#include "Player.h"
#include <fstream>
#include <map>

namespace Engine
{
	void PickupManager::loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, std::shared_ptr<EffectManager> effectManager)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/pickups.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Pickups");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Pickup"); brewery_node; brewery_node = brewery_node->next_sibling("Pickup"))
		{
			std::string name = brewery_node->first_attribute("name")->value();
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto _pickup = std::make_shared<Pickup>(22.0f, 21.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			_pickup->applyAnimation(sprite);
			_pickup->effect = effectManager->getEffect(name);

			pickups.push_back(pickup(name, std::move(_pickup)));
		}
	}

	std::shared_ptr<Pickup> PickupManager::getPickup(std::string index)
	{
		for (auto pickup : pickups)
		{
			if (pickup.first == index)
			{
				auto _pickup = std::make_shared<Pickup>(*pickup.second);
				_pickup->onCollision = [_pickup](std::shared_ptr<BaseGameObject> collider)
				{
					if(_pickup->effect(collider)) _pickup->setNeedsToBeDeleted(true);
				};
				return _pickup;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Pickup> PickupManager::getRandomPickup()
	{
		int randIndex = rand() % pickups.size();
		auto _pickup = std::make_shared<Pickup>(*pickups[randIndex].second);
		_pickup->onCollision = [_pickup](std::shared_ptr<BaseGameObject> collider)
		{
			if (_pickup->effect(collider)) _pickup->setNeedsToBeDeleted(true);
		};
		return _pickup;
	}
}