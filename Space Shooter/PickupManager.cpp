#include "PickupManager.h"
#include "Entity.h"
#include <fstream>

namespace Engine
{
	void PickupManager::loadPickupsFromConfig(std::shared_ptr<SpriteSheetManager> spriteSheetManager)
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
			std::string pickupSound = brewery_node->first_attribute("pickupSound")->value();
			std::string name = brewery_node->first_attribute("name")->value();
			auto spriteName = brewery_node->first_attribute("spriteName")->value();
			auto sprite = spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName);
			auto _pickup = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			_pickup->applyAnimation(sprite);

			for (auto brewery_node2 = brewery_node->first_node("ShootingEffect"); brewery_node2; brewery_node2 = brewery_node2->next_sibling("ShootingEffect"))
			{
				std::string explosionSound = brewery_node2->first_attribute("explosionSound")->value();
				std::string::size_type sz;
				float delayBetweenShoots = std::stof(brewery_node2->first_attribute("delayBetweenShoots")->value(), &sz);
				std::vector<glm::vec2> bulletPositions;

				auto bulletOffset = glm::vec2(1.0f, 1.0f);

				for (auto brewery_node3 = brewery_node2->first_node("Array"); brewery_node3; brewery_node3 = brewery_node3->next_sibling("Array"))
				{
					bulletOffset.x = 1.0f;
					for (auto brewery_node4 = brewery_node3->first_node("Element"); brewery_node4; brewery_node4 = brewery_node4->next_sibling("Element"))
					{
						int value = std::stoi(brewery_node4->value());
						if (value == 1)
						{
							bulletPositions.push_back(bulletOffset);
						}
						bulletOffset.x += 1.0f;
					}
					bulletOffset.y += 2.0f;
				}

				_pickup->onCollision = [explosionSound, bulletPositions, delayBetweenShoots, pickupSound, _pickup](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					_pickup->setNeedsToBeRemoved(true);

					entity->setDelayBetweenShoots(delayBetweenShoots);
					entity->setDelayBetweenShootsTimer(0.0f);

					entity->shootingMode = [explosionSound, bulletPositions, delayBetweenShoots](Entity* entity)
					{
						auto params = std::vector<std::pair<std::string, BaseGameObject*>>();
						params.push_back(std::pair<std::string, BaseGameObject*>(entity->getShootingSound(), nullptr));
						entity->notify(ObserverEvent::BULLETSHOT, params);

						for (auto bulletPosition : bulletPositions)
						{
							auto bullet = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 200.0f) * entity->getShootingDirection(), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
							bullet->applyAnimation(entity->getAnimationByIndex("shoot"));
							bullet->setScale(0.5f);
							bullet->onUpdate = [bullet]()
							{
								float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
								float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

								//Collision detection
								if (bullet->getPosition(0) + bullet->getWidth() >= windowWidth || bullet->getPosition(0) <= 0.0f)
									bullet->setNeedsToBeRemoved(true);

								if (bullet->getPosition(1) + bullet->getHeight() >= windowHeigth || bullet->getPosition(1) <= 0.0f)
									bullet->setNeedsToBeRemoved(true);
							};

							auto _bulletPosition = entity->getShootingPosition();
							_bulletPosition.x -= (bullet->getWidth() / 2.0f);
							_bulletPosition += glm::vec2(bullet->getWidth() * bulletPosition.x, bullet->getHeight() * bulletPosition.y) * entity->getShootingDirection();

							bullet->setPosition(_bulletPosition);

							entity->addBullet(std::move(bullet));
						}
					};
					return true;
				};
			}

			for (auto brewery_node2 = brewery_node->first_node("Shield"); brewery_node2; brewery_node2 = brewery_node2->next_sibling("Shield"))
			{
				std::string loseSound = brewery_node2->first_attribute("loseSound")->value();

				_pickup->onCollision = [loseSound, spriteSheetManager, _pickup](std::shared_ptr<BaseGameObject> collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					_pickup->setNeedsToBeRemoved(true);

					auto shield = std::make_shared<BaseGameObject>(0.0f, 0.0f, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					shield->setScale(0.5f);
					shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite("shield3.png"));
					shield->onUpdate = [shield, entity, spriteSheetManager]()
					{
						shield->setPosition(entity->getPosition() + glm::vec2(((shield->getWidth() - entity->getWidth()) * -1.0f) / 2.0f, ((shield->getHeight() - entity->getHeight()) * -1.0f) / 2.0f));
					};
					entity->addAddon(addon("shield", std::move(shield)));
					return true;
				};
			}
			pickups.push_back(pickup(name, std::move(_pickup)));
		}
	}

	std::shared_ptr<BaseGameObject> PickupManager::getPickup(std::string index)
	{
		for (auto pickup : pickups)
		{
			if (pickup.first == index)
			{
				return std::make_shared<BaseGameObject>(*pickup.second);
			}
		}
		return nullptr;
	}

	std::shared_ptr<BaseGameObject> PickupManager::getRandomPickup()
	{
		int randIndex = rand() % pickups.size();
		for(size_t i = 0; i < pickups.size(); i++)
		{
			if (i == randIndex)
			{
				return std::make_shared<BaseGameObject>(*pickups[i].second);
			}
		}
		return nullptr;
	}
}