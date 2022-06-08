#include "SpriteSheetManager.hpp"
#include "SpriteSheet.hpp"

#include <fstream>
#include "rapidxml/rapidxml.hpp"
#include <algorithm>

namespace Engine
{
	void SpriteSheetManager::loadSpriteSheetsFromConfig()
	{
		auto doc = new rapidxml::xml_document<>();
		// Read the xml file into a vector
		std::ifstream theFile("assets/Config/spritesheets.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(&buffer[0]);
		// Find our root node
		rapidxml::xml_node<>* root_node = doc->first_node("Spritesheets");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Spritesheet"); brewery_node; brewery_node = brewery_node->next_sibling("Spritesheet"))
		{
			auto spriteSheet = std::make_shared<SpriteSheet>(brewery_node->first_attribute("spriteSheetName")->value(), brewery_node->first_attribute("spriteSheetNameXml")->value());

			for (auto beer_node = brewery_node->first_node("Sprites"); beer_node; beer_node = beer_node->next_sibling("Sprites"))
			{
				std::vector<glm::vec4> sprites;
				for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
				{
					sprites.push_back(spriteSheet->getSpriteAsVector(beer_node2->first_attribute("name")->value()));
				}

				spriteSheet->makeAnimation(beer_node->first_attribute("name")->value(), sprites);
			}

			loadSpriteSheet(brewery_node->first_attribute("name")->value(), spriteSheet);
		}

		theFile.close();
		doc->clear();
		delete doc;
	}

	void SpriteSheetManager::loadSpriteSheet(const std::string& name, const std::shared_ptr<SpriteSheet>& t_spriteSheet)
	{
		auto spriteSheets = getSpriteSheets();

		for (const auto& spriteSheet : *spriteSheets)
		{
			if (spriteSheet.first == name)
			{
				return;
			}
		}

		spriteSheets->push_back(spriteSheet(name, t_spriteSheet));
	}

	std::shared_ptr<SpriteSheet> SpriteSheetManager::getSpriteSheet(const std::string& name)
	{
		auto spriteSheets = *getSpriteSheets();
		const auto it = std::find_if(spriteSheets.begin(), spriteSheets.end(), [name](auto keyname) { return keyname.first == name; });

		return it != spriteSheets.end() ? it->second : nullptr;
	}
}
