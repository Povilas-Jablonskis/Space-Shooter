#include "EffectManager.h"
#include "Entity.h"
#include "Player.h"
#include <fstream>
#include <map>

namespace Engine
{
	void EffectManager::loadEffectsFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager, irrklang::ISoundEngine* soundEngine)
	{
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/effects.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Effects");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Effect"); brewery_node; brewery_node = brewery_node->next_sibling("Effect"))
		{
			for (auto beer_node = brewery_node->first_node("ShootingEffect"); beer_node; beer_node = beer_node->next_sibling("ShootingEffect"))
			{
				auto list = std::map<std::string, ShootingType>();
				list["NORMAL"] = ShootingType::NORMAL;
				list["DOUBLE"] = ShootingType::DOUBLE;
				list["HALFCIRCLE"] = ShootingType::HALFCIRCLE;
				list["DOUBLEHALFCIRCLE"] = ShootingType::DOUBLEHALFCIRCLE;
				list["NONE"] = ShootingType::NONE;

				std::string sound = beer_node->first_attribute("sound")->value();
				std::string::size_type sz;
				std::string shootingType = std::string(beer_node->first_attribute("shootingType")->value());
				float delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value(), &sz);

				auto effect = [soundEngine, list, sound, shootingType, delayBetweenShoots](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());
					auto player = dynamic_cast<Player*>(collider.get());

					if (player != nullptr)
						soundEngine->play2D(sound.c_str(), GL_FALSE);

					entity->setDelayBetweenShoots(delayBetweenShoots);
					entity->setShootingType(list.at(shootingType));
					return true;
				};
				effects.push_back(std::pair<std::string, std::function<bool(std::shared_ptr<BaseGameObject>)>>(brewery_node->first_attribute("name")->value(), std::move(effect)));
			}

			for (auto beer_node = brewery_node->first_node("Shield"); beer_node; beer_node = beer_node->next_sibling("Shield"))
			{
				std::string sound = beer_node->first_attribute("sound")->value();

				auto effect = [soundEngine, beer_node, spriteSheetManager, sound](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());
					auto player = dynamic_cast<Player*>(collider.get());

					if (entity->getAddon("shield") != nullptr) return false;

					if (player != nullptr)
						soundEngine->play2D(sound.c_str(), GL_FALSE);

					auto shield = std::make_shared<Addon>(48.0f, 48.0f, glm::vec2(-8.0f, -6.0f));
					shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getAnimation("shieldSpriteSheet"));
					entity->addAddon(std::pair<std::string, std::shared_ptr<Addon>>("shield", std::move(shield)));
					return true;
				};

				effects.push_back(std::pair<std::string, std::function<bool(std::shared_ptr<BaseGameObject>)>>(brewery_node->first_attribute("name")->value(), std::move(effect)));
			}
		}
	}


	std::function<bool(std::shared_ptr<BaseGameObject>)> EffectManager::getEffect(std::string index)
	{
		for (auto effect : effects)
		{
			if (effect.first == index)
				return effect.second;
		}

		return nullptr;
	}

	std::function<bool(std::shared_ptr<BaseGameObject>)> EffectManager::getRandomEffect()
	{
		int randIndex = rand() % effects.size();
		return effects[randIndex].second;
	}
}