#include "SpriteSheetManager.h"
#include <fstream>
#include "rapidxml.hpp"

namespace Engine
{
	SpriteSheetManager::~SpriteSheetManager()
	{
		spriteSheets.clear();
	}

	void SpriteSheetManager::loadSpriteSheetsFromConfig()
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/spritesheets.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Spritesheets");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Spritesheet"); brewery_node; brewery_node = brewery_node->next_sibling("Spritesheet"))
		{
			auto spriteSheet = std::make_shared<SpriteSheet>();

			spriteSheet->loadSpriteSheet(brewery_node->first_attribute("spriteSheetName")->value());
			spriteSheet->loadSpritesFromXml(brewery_node->first_attribute("spriteSheetNameXml")->value());

			for (auto beer_node = brewery_node->first_node("Sprites"); beer_node; beer_node = beer_node->next_sibling("Sprites"))
			{
				std::vector<glm::vec4> sprites;
				for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
				{
					sprites.push_back(std::move(spriteSheet->getSpriteAsVector(beer_node2->first_attribute("name")->value())));
				}

				spriteSheet->makeAnimation(beer_node->first_attribute("name")->value(), sprites);
				spriteSheet->getAnimation(beer_node->first_attribute("name")->value())->setLoopStatus(std::stoi(beer_node->first_attribute("loop")->value()));
				spriteSheet->getAnimation(beer_node->first_attribute("name")->value())->setDelay(std::stof(beer_node->first_attribute("delay")->value()));
			}

			loadSpriteSheet(brewery_node->first_attribute("name")->value(), std::move(spriteSheet));
		}
	}

	void SpriteSheetManager::loadSpriteSheet(const std::string& name, std::shared_ptr<SpriteSheet> _spriteSheet)
	{
		for (auto spriteSheet : spriteSheets)
		{
			if (spriteSheet.first == name)
				return;
		}
		spriteSheets.push_back(std::move(spriteSheet(name, _spriteSheet)));
	}

	std::shared_ptr<SpriteSheet> SpriteSheetManager::getSpriteSheet(const std::string& name)
	{
		for (auto spriteSheet : spriteSheets)
		{
			if (spriteSheet.first == name)
				return spriteSheet.second;
		}
		return nullptr;
	}
}