#include "EffectManager.h"
#include "Entity.h"
#include "Player.h"
#include <fstream>
#include <map>

#define _USE_MATH_DEFINES
#include <math.h>

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
				auto list = std::map<std::string, std::function<void(Entity*)>>();
				list["NORMAL"] = [](Entity* entity)
				{
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					auto params = std::map<std::string, BaseGameObject*>();
					params["collider"] = entity;
					entity->notify(ObserverEvent::BULLETSHOT, params);
				};

				list["DOUBLE"] = [](Entity* entity)
				{
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					auto params = std::map<std::string, BaseGameObject*>();
					params["collider"] = entity;
					entity->notify(ObserverEvent::BULLETSHOT, params);
				};

				list["HALFCIRCLE"] = [](Entity* entity)
				{
					//Middle
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					entity->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 20.0f, entity->getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 40.0f, entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 20.0f, entity->getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 40.0f, entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					auto params = std::map<std::string, BaseGameObject*>();
					params["collider"] = entity;
					entity->notify(ObserverEvent::BULLETSHOT, params);
				};

				list["DOUBLEHALFCIRCLE"] = [](Entity* entity)
				{
					////////////1

					//Middle
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 20.0f - 4.5f - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 20.0f, entity->getPosition(1) - 20.0f + 4.5f - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 40.0f, entity->getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 20.0f, entity->getPosition(1) - 20.0f - 4.5f - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 40.0f, entity->getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					////////////2

					//Middle
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f), entity->getPosition(1) - 20.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					//Right side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 20.0f, entity->getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) - 40.0f, entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					//Left side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 20.0f, entity->getPosition(1) - 15.0f - 4.5f - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(entity->getPosition(0) + (entity->getSize(0) / 2.0f) + 40.0f, entity->getPosition(1) - 4.5f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", entity->getAnimationByIndex("explosion"));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					entity->addBullet(std::move(bullet));

					auto params = std::map<std::string, BaseGameObject*>();
					params["collider"] = entity;
					entity->notify(ObserverEvent::BULLETSHOT, params);
				};

				std::string sound = beer_node->first_attribute("sound")->value();
				std::string::size_type sz;
				std::string shootingMode = std::string(beer_node->first_attribute("shootingMode")->value());
				float delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value(), &sz);

				auto effect = [soundEngine, shootingMode = list[shootingMode], sound, delayBetweenShoots](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());
					auto player = dynamic_cast<Player*>(collider.get());

					if (player != nullptr)
						soundEngine->play2D(sound.c_str(), GL_FALSE);

					entity->setDelayBetweenShoots(delayBetweenShoots);
					entity->shootingMode = shootingMode;
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