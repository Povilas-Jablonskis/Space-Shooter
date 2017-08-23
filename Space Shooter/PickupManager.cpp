#include "PickupManager.h"
#include <fstream>

namespace Engine
{
	void PickupManager::loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, std::map<std::string, std::function<void(BaseGameObject*)>> effects)
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
		root_node = doc.first_node();
		// Iterate over the brewerys
		for (rapidxml::xml_node<> * brewery_node = root_node->first_node(); brewery_node; brewery_node = brewery_node->next_sibling())
		{
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			auto effect = effects.find(brewery_node->first_attribute("effect")->value());
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto pickup = std::make_shared<Pickup>(22.0f, 21.0f, glm::vec2(0.0f, 0.0f));
			pickup->applyAnimation(sprite);
			if (effect != effects.end())
				pickup->setEffect(effect->second);
			pickups.insert(std::pair<std::string, std::shared_ptr<Pickup>>(brewery_node->first_attribute("name")->value(), std::move(pickup)));
		}
	}

	std::shared_ptr<Pickup> PickupManager::getPickup(std::string index)
	{
		auto pickup = pickups.find(index);
		if (pickup == pickups.end())
			return nullptr;

		return std::make_shared<Pickup>(*pickup->second);
	}
}