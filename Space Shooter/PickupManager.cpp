#include "PickupManager.h"
#include <fstream>

namespace Engine
{
	void PickupManager::loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, std::shared_ptr<EffectManager> effectManager)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("pickups.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Pickups");
		// Iterate over the brewerys
		for (rapidxml::xml_node<> * brewery_node = root_node->first_node("Pickup"); brewery_node; brewery_node = brewery_node->next_sibling("Pickup"))
		{
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto pickup = std::make_shared<BaseGameObject>(22.0f, 21.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			auto effect = effectManager->getEffect(brewery_node->first_attribute("effect")->value());
			pickup->applyAnimation(sprite);
			pickup->collisionEffectEntity = [effect, pickup](Entity* collider)
			{
				if (collider != nullptr && !collider->getNeedsToBeDeleted())
					effect(collider);
			};
			pickups.push_back(std::pair<std::string, std::shared_ptr<BaseGameObject>>(brewery_node->first_attribute("name")->value(), std::move(pickup)));
		}
	}

	std::shared_ptr<BaseGameObject> PickupManager::getPickup(std::string index)
	{
		for (auto pickup : pickups)
		{
			if (pickup.first == index)
				return std::make_shared<BaseGameObject>(*pickup.second);
		}
		return nullptr;
	}

	std::shared_ptr<BaseGameObject> PickupManager::getRandomPickup()
	{
		int randIndex = rand() % pickups.size();
		return std::make_shared<BaseGameObject>(*pickups[randIndex].second);
	}
}