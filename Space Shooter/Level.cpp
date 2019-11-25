#include "Level.hpp"
#include "Text.hpp"
#include "Player.hpp"
#include "GameStateManager.hpp"
#include "ConfigurationManager.hpp"
#include "SpriteSheetManager.hpp"
#include "CollisionManager.hpp"
#include "SpriteSheet.hpp"
#include "Renderer.hpp"
#include "UIManager.hpp"

namespace Engine
{
	Level::~Level()
	{

	}

	Level::Level(rapidxml::xml_node<char>* t_xml_node, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager, irrklang::ISoundEngine* soundEngine, int t_characterSelectionIndex)
	{
		m_background = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(0.0f, 0.0f));
		m_background->applyAnimation(spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));

		for (auto beer_node = t_xml_node->first_node("Enemy"); beer_node; beer_node = beer_node->next_sibling("Enemy"))
		{
			auto positionX = std::stof(beer_node->first_attribute("positionX")->value());
			auto positionY = std::stof(beer_node->first_attribute("positionY")->value());
			auto velocityX = std::stof(beer_node->first_attribute("velocityX")->value());
			auto velocityY = std::stof(beer_node->first_attribute("velocityY")->value());
			std::string spriteName = beer_node->first_attribute("spriteName")->value();

			auto enemy = std::make_shared<Entity>(glm::vec2(positionX, positionY), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			enemy->addObserver(this);
			enemy->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
			enemy->setValue(std::stoi(beer_node->first_attribute("value")->value()));

			for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
			{
				std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
				std::vector<std::shared_ptr<BaseGameObject>> bullets;
				auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
				std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

				for (auto beer_node3 = beer_node2->first_node("Bullet"); beer_node3; beer_node3 = beer_node3->next_sibling("Bullet"))
				{
					auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					bullet->addObserver(this);
					bullet->setExplosionSound(explosionSound);
					bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

					for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
					{
						for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
						{
							bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
						}
						for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
						{
							bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
						}
					}

					bullets.push_back(bullet);
				}

				enemy->shootingModeFunc = [soundEngine, shootingSound, bullets, enemy, explosionSound]()
				{
					for (auto bullet : bullets)
					{
						auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
						auto _bulletPosition = glm::vec2(enemy->getPosition().x + (enemy->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), enemy->getPosition().y - (enemy->getHeight() / 2.0f));
						_bulletPosition += glm::vec2(enemy->getWidth() * _bullet->getPosition().x, enemy->getHeight() * _bullet->getPosition().y);
						_bullet->setPosition(_bulletPosition);
						
						_bullet->onUpdateFunc = [_bullet]()
						{
							auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
							auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

							//Collision detection
							if (_bullet->getPosition().y > windowHeight || (_bullet->getPosition().y + _bullet->getHeight()) < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
							{
								_bullet->setNeedsToBeRemoved(true);
							}
						};
						
						enemy->addBullet(_bullet);
					}

					soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
				};
				enemy->setDelayBetweenShoots(delayBetweenShoots);
			}

			for (auto beer_node2 = beer_node->first_node("Animations"); beer_node2; beer_node2 = beer_node2->next_sibling("Animations"))
			{
				for (auto beer_node3 = beer_node2->first_node("Sprite"); beer_node3; beer_node3 = beer_node3->next_sibling("Sprite"))
				{
					enemy->addAnimation(beer_node3->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));
				}

				for (auto beer_node3 = beer_node2->first_node("Animation"); beer_node3; beer_node3 = beer_node3->next_sibling("Animation"))
				{
					enemy->addAnimation(beer_node3->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node3->first_attribute("animationName")->value()));
				}
			}

			enemy->onUpdateFunc = [enemy, this]()
			{
				auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				//Collision detection
				if (enemy->getPosition().x + enemy->getWidth() >= windowWidth || enemy->getPosition().x <= 0.0f)
					enemy->setVelocity(0, enemy->getVelocity().x * -1.0f);

				if (enemy->getPosition().y + enemy->getHeight() >= windowHeigth || enemy->getPosition().y <= 0.0f)
					enemy->setVelocity(1, enemy->getVelocity().y * -1.0f);
			};

			m_enemies.push_back(enemy);
		}

		for (auto beer_node = t_xml_node->first_node("Meteor"); beer_node; beer_node = beer_node->next_sibling("Meteor"))
		{
			auto meteor = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			meteor->addObserver(this);
			meteor->setExplosionSound(beer_node->first_attribute("explosionSound")->value());
			meteor->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
			meteor->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node->first_attribute("spriteName")->value()));
			meteor->onUpdateFunc = [meteor]()
			{
				meteor->setRotationAngle(meteor->getRotationAngle() + 0.05f);

				if (meteor->getRotationAngle() >= (3.14159265358979323846f * 2.0f))
				{
					meteor->setRotationAngle(0.0f);
				}
			};

			meteor->setValue(100);
			m_meteors.push_back(meteor);
		}

		for (auto beer_node = t_xml_node->first_node("Pickup"); beer_node; beer_node = beer_node->next_sibling("Pickup"))
		{
			std::string spriteName = beer_node->first_attribute("spriteName")->value();
			auto pickup = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
			pickup->setExplosionSound(beer_node->first_attribute("pickupSound")->value());
			pickup->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
			pickup->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));

			for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
			{
				std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
				std::vector<std::shared_ptr<BaseGameObject>> bullets;
				auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
				std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

				for (auto beer_node3 = beer_node2->first_node("Bullet"); beer_node3; beer_node3 = beer_node3->next_sibling("Bullet"))
				{
					auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
					bullet->addObserver(this);
					bullet->setExplosionSound(explosionSound);
					bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

					for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
					{
						for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
						{
							bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
						}
						for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
						{
							bullet->addAnimation(beer_node5->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
						}
					}

					bullets.push_back(bullet);
				}

				pickup->onCollisionFunc = [soundEngine, bullets, explosionSound, shootingSound, delayBetweenShoots, pickup, this](const std::shared_ptr<BaseGameObject>& collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					pickup->setNeedsToBeRemoved(true);

					entity->shootingModeFunc = [soundEngine, bullets, explosionSound, shootingSound, entity, this]()
					{
						for (auto bullet : bullets)
						{
							auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
							auto _bulletPosition = glm::vec2(entity->getPosition().x + (entity->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), entity->getPosition().y + (entity->getHeight() / 2.0f));
							_bulletPosition += glm::vec2(entity->getWidth() * _bullet->getPosition().x, entity->getHeight() * _bullet->getPosition().y);
							_bullet->setPosition(_bulletPosition);

							_bullet->onUpdateFunc = [_bullet]()
							{
								auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
								auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

								//Collision detection
								if (_bullet->getPosition().y > windowHeight || (_bullet->getPosition().y + _bullet->getHeight()) < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
								{
									_bullet->setNeedsToBeRemoved(true);
								}
							};

							entity->addBullet(_bullet);
						}

						soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
					};

					entity->setDelayBetweenShoots(delayBetweenShoots);
					entity->setDelayBetweenShootsTimer(0.0f);
					return true;
				};
			}

			for (auto beer_node2 = beer_node->first_node("Shield"); beer_node2; beer_node2 = beer_node2->next_sibling("Shield"))
			{
				std::string spriteName = beer_node2->first_attribute("spriteName")->value();

				pickup->onCollisionFunc = [&spriteSheetManager, spriteName, pickup, this](const std::shared_ptr<BaseGameObject>& collider)
				{
					auto entity = dynamic_cast<Entity*>(collider.get());

					pickup->setNeedsToBeRemoved(true);

					auto shield = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.7f));

					shield->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
					shield->onUpdateFunc = [shield, entity]()
					{
						shield->changeColor(shield->getColor().a - 0.05f, 3);

						if (shield->getColor().a <= 0.0f)
						{
							shield->changeColor(0.7f, 3);
						}

						shield->setPosition(entity->getPosition() + glm::vec2(((shield->getWidth() - entity->getWidth()) * -1.0f) / 2.0f, ((shield->getHeight() - entity->getHeight()) * -1.0f) / 2.0f));
					};
					entity->addAddon(addon("shield", shield));
					return true;
				};
			}

			m_pickups.push_back(pickup);
		}

		size_t i = 0;
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<> * root_node;
		// Read the xml file into a vector
		std::ifstream theFile("Config/players.xml");
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc.first_node("Players");
		// Iterate over the brewerys
		for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->next_sibling("Player"))
		{
			if (i == t_characterSelectionIndex)
			{
				auto lives = std::stoi(brewery_node->first_attribute("lives")->value());
				std::string livesIcon = brewery_node->first_attribute("livesIcon")->value();
				std::string spriteName = brewery_node->first_attribute("spriteName")->value();
				auto velocityX = std::stof(brewery_node->first_attribute("velocityX")->value());
				auto velocityY = std::stof(brewery_node->first_attribute("velocityY")->value());

				m_player = std::make_shared<Player>(glm::vec2(static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)) / 2.0f, 0.0f), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				m_player->addObserver(this);
				m_player->setLivesIcon(livesIcon);
				m_player->setScore(0);
				m_player->setLives(lives);
				m_player->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));

				for (auto beer_node = brewery_node->first_node("ShootingEffect"); beer_node; beer_node = beer_node->next_sibling("ShootingEffect"))
				{
					std::string explosionSound = beer_node->first_attribute("explosionSound")->value();
					std::vector<std::shared_ptr<BaseGameObject>> bullets;
					auto delayBetweenShoots = std::stof(beer_node->first_attribute("delayBetweenShoots")->value());
					std::string shootingSound = beer_node->first_attribute("shootingSound")->value();

					for (auto beer_node2 = beer_node->first_node("Bullet"); beer_node2; beer_node2 = beer_node2->next_sibling("Bullet"))
					{
						auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node2->first_attribute("offsetX")->value()), std::stof(beer_node2->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node2->first_attribute("velocityX")->value()), std::stof(beer_node2->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
						bullet->addObserver(this);
						bullet->setExplosionSound(explosionSound);
						bullet->applyAnimation(spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));

						for (auto beer_node3 = beer_node2->first_node("Animations"); beer_node3; beer_node3 = beer_node3->next_sibling("Animations"))
						{
							for (auto beer_node4 = beer_node3->first_node("Sprite"); beer_node4; beer_node4 = beer_node4->next_sibling("Sprite"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node4->first_attribute("spriteName")->value()));
							}
							for (auto beer_node4 = beer_node3->first_node("Animation"); beer_node4; beer_node4 = beer_node4->next_sibling("Animation"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node4->first_attribute("animationName")->value()));
							}
						}

						bullets.push_back(bullet);
					}

					m_player->shootingModeFunc = [soundEngine, bullets, explosionSound, shootingSound, &spriteSheetManager, this]()
					{
						for (auto bullet : bullets)
						{
							auto _bullet = std::make_shared<BaseGameObject>(*bullet.get());
							auto _bulletPosition = glm::vec2(m_player->getPosition().x + (m_player->getWidth() / 2.0f) - (_bullet->getWidth() / 2.0f), m_player->getPosition().y + (m_player->getHeight() / 2.0f));
							_bulletPosition += glm::vec2(m_player->getWidth() * _bullet->getPosition().x, m_player->getHeight() * _bullet->getPosition().y);
							_bullet->setPosition(_bulletPosition);

							_bullet->onUpdateFunc = [_bullet]()
							{
								auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
								auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

								//Collision detection
								if (_bullet->getPosition().y > windowHeight || (_bullet->getPosition().y + _bullet->getHeight()) < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
								{
									_bullet->setNeedsToBeRemoved(true);
								}
							};

							m_player->addBullet(_bullet);
						}

						soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
					};
					m_player->setDelayBetweenShoots(delayBetweenShoots);
				}

				for (auto beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
				{
					for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
					{
						m_player->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));
					}

					for (auto beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
					{
						m_player->addAnimation(beer_node2->first_attribute("name")->value(), spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node2->first_attribute("animationName")->value()));
					}
				}
			}
			++i;
		}

		theFile.close();
		doc.clear();
	}

	bool Level::update(float dt, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<InputManager>& inputManager, const std::unique_ptr<CollisionManager>& collisionManager)
	{
		auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
		auto frameTime = (newTime - m_currentTime) / 1000.0f;
		m_currentTime = newTime;

		m_accumulator += frameTime;

		while (m_accumulator >= dt)
		{
			if (gameStateManager->getGameState() != GameState::IN_MENU && gameStateManager->getGameState() != GameState::IN_PAUSED_MENU)
			{
				m_player->update(dt, inputManager);

				for (auto it = m_enemies.begin(); it != m_enemies.end();)
				{
					if ((*it)->update(dt))
					{
						it = m_enemies.erase(it);
					}
					else
					{
						++it;
					}
				}

				for (auto it = m_explosions.begin(); it != m_explosions.end();)
				{
					if ((*it)->update(dt))
					{
						it = m_explosions.erase(it);
					}
					else
					{
						++it;
					}
				}

				for (auto it = m_pickups.begin(); it != m_pickups.end();)
				{
					if ((*it)->update(dt))
					{
						it = m_pickups.erase(it);
					}
					else
					{
						++it;
					}
				}

				for (auto it = m_meteors.begin(); it != m_meteors.end();)
				{
					if ((*it)->update(dt))
					{
						it = m_meteors.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			if (gameStateManager->getGameState() == GameState::STARTED)
			{
				for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
				{
					auto playerBulletList = m_player->getBulletsList();
					for (auto it2 = playerBulletList->begin(); it2 != playerBulletList->end(); ++it2)
					{
						collisionManager->checkCollision(*it2, (*it)->getBulletsList());
					}
					collisionManager->checkCollision(m_player, (*it)->getBulletsList());
					collisionManager->checkCollision(*it, m_player->getBulletsList());
				}

				for (auto it = m_meteors.begin(); it != m_meteors.end(); ++it)
				{
					collisionManager->checkCollision(*it, m_player->getBulletsList());
				}

				collisionManager->checkCollision(m_player, &m_meteors);
				collisionManager->checkCollision(m_player, &m_enemies);
				collisionManager->checkCollision(m_player, &m_pickups);
			}

			m_accumulator -= dt;
		}

		return m_meteors.empty() && m_enemies.empty() && m_pickups.empty();
	}

	void Level::render(float dt, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<InputManager>& inputManager, const std::unique_ptr<CollisionManager>& collisionManager, const std::unique_ptr<Renderer>& renderer, const std::unique_ptr<ConfigurationManager>& configurationManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		if (gameStateManager->getGameState() != GameState::IN_MENU && gameStateManager->getGameState() != GameState::IN_PAUSED_MENU)
		{
			//Render m_background
			m_background->update(dt, inputManager);
			renderer->draw(m_background);
			//Render m_meteors
			renderer->draw(m_meteors);
			//Render m_player & his addons
			renderer->draw(m_player);
			for (auto it = m_player->getAddons()->begin(); it != m_player->getAddons()->end(); ++it)
			{
				renderer->draw(it->second);
			}
			//Render m_enemies & their addons
			renderer->draw(m_enemies);
			for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
			{
				for (auto it2 = (*it)->getAddons()->begin(); it2 != (*it)->getAddons()->end(); ++it2)
				{
					renderer->draw(it2->second);
				}
			}
			//Render m_pickups
			renderer->draw(m_pickups);
			//Render m_explosions
			renderer->draw(m_explosions);
			//Render m_enemies bullets
			for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it)
			{
				renderer->draw(*(*it)->getBulletsList());
			}
			//Render m_player's bullets
			renderer->draw(*m_player->getBulletsList());
			//Render & Update UI elements
			getUIManager()->updatePlayerLives(spriteSheetManager, m_player->getLivesIcon(), m_player->getLives());
			getUIManager()->updatePlayerScore(spriteSheetManager, m_player->getScore());

			getUIManager()->render(dt, gameStateManager, inputManager, renderer, configurationManager);
		}
	}

	void Level::onNotify(ObserverEvent event)
	{
		switch (event)
		{
			case PLAYER_DIED:
			{
				auto option = std::make_shared<Text>("Game Over", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(50.0f, 55.0f));
				option->disable();
				//m_notifications.push_back(uiPlayerElement("Game Over", option));

				//getGameStateManager()->setGameState(GameState::ENDED);
				break;
			}
		}
	}

	void Level::onNotify(ObserverEvent event, BaseGameObject* obj)
	{
		switch (event)
		{
			case OBJECT_DESTROYED:
			{
				m_player->setScore(m_player->getScore() + obj->getValue());

				if (!obj->getExplosionSound().empty())
				{
					//getSoundEngine()->play2D(obj->getExplosionSound().c_str(), GL_FALSE);
				}

				if (obj->getAnimationByIndex("explosion") == nullptr)
				{
					return;
				}

				auto explosion = std::make_shared<BaseGameObject>(obj->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				explosion->applyAnimation(obj->getAnimationByIndex("explosion"));
				explosion->onUpdateFunc = [explosion]()
				{
					explosion->changeColor(explosion->getColor().a - 0.05f, 3);

					if (explosion->getColor().a <= 0.0f)
					{
						explosion->setNeedsToBeRemoved(true);
					}
				};

				m_explosions.push_back(explosion);
				break;
			}
		}
	}
}