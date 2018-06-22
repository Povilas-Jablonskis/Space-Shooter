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
				std::string explosionSound = beer_node->first_attribute("explosionSound")->value();
				auto list = std::map<std::string, std::function<void(Entity*)>>();
				list["NORMAL"] = [explosionSound, soundEngine](Entity* entity)
				{
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2(0.0f, 0.0f));
				};

				list["DOUBLE"] = [explosionSound, soundEngine](Entity* entity)
				{
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2(0.0f, 0.0f));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2(0.0f, bullet->getSize(1) * 2.0f));
				};

				list["HALFCIRCLE"] = [explosionSound, soundEngine](Entity* entity)
				{
					//Middle
					auto bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2(0.0f, 0.0f));

					//Right side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2((bullet->getSize(0) * 2.0f), 0.0f));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2((bullet->getSize(0) * 4.0f), bullet->getSize(1) * -1.0f));

					//Left side
					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2((bullet->getSize(0) * 2.0f) * -1.0f, 0.0f));

					bullet = std::make_shared<BaseGameObject>(9.0f, 20.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 69.0f, 0.0f, 1.0f));
					bullet->addAnimation("explosion", std::move(entity->getAnimationByIndex("explosion")));
					bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
					bullet->onDeath = [explosionSound, soundEngine]()
					{
						soundEngine->play2D(explosionSound.c_str(), GL_FALSE);
					};
					entity->addBullet(std::move(bullet), glm::vec2((bullet->getSize(0) * 4.0f) * -1.0f, bullet->getSize(1) * -1.0f));
				};

				std::string::size_type sz;
				std::string shootingMode = std::string(beer_node->first_attribute("shootingMode")->value());
				float delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value(), &sz);

				auto _effect = [soundEngine, shootingMode = list[shootingMode], delayBetweenShoots](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					entity->setDelayBetweenShoots(delayBetweenShoots);
					entity->shootingMode = shootingMode;
					return true;
				};
				effects.push_back(std::move(effect(brewery_node->first_attribute("name")->value(), std::move(_effect))));
			}

			for (auto beer_node = brewery_node->first_node("Shield"); beer_node; beer_node = beer_node->next_sibling("Shield"))
			{
				std::string loseSound = beer_node->first_attribute("loseSound")->value();

				auto _effect = [soundEngine, loseSound, spriteSheetManager](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					auto shield = std::make_shared<Addon>(48.0f, 48.0f, glm::vec2(-8.0f, -8.0f));
					shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getAnimation("shieldSpriteSheet"));
					shield->onDeath= [soundEngine, loseSound]()
					{
						soundEngine->play2D(loseSound.c_str(), GL_FALSE);
					};
					entity->addAddon(std::move(addon("shield", std::move(shield))));
					return true;
				};

				effects.push_back(std::move(effect(brewery_node->first_attribute("name")->value(), std::move(_effect))));
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