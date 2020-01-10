#include "LevelManager.hpp"
#include "SpriteSheetManager.hpp"
#include "SpriteSheet.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "ConfigurationManager.hpp"
#include "CollisionManager.hpp"
#include "Player.hpp"

#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <iostream>

namespace Engine
{
	LevelManager::LevelManager(const std::unique_ptr<SpriteSheetManager>& spriteSheetManager, irrklang::ISoundEngine* soundEngine, int t_characterSelectionIndex)
	{
		{
			rapidxml::xml_document<> doc;
			rapidxml::xml_node<> * root_node;
			// Read the xml file into a vector
			std::ifstream theFile("Config/levels.xml");
			std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
			buffer.push_back('\0');
			// Parse the buffer using the xml file parsing library into doc 
			doc.parse<0>(&buffer[0]);
			// Find our root node
			root_node = doc.first_node("Levels");
			// Iterate over the brewerys
			for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->next_sibling("Level"))
			{
				m_levels.push(std::make_unique<Level>(brewery_node, spriteSheetManager, soundEngine, t_characterSelectionIndex));
			}

			theFile.close();
			doc.clear();
		}

		{
			rapidxml::xml_document<> doc;
			rapidxml::xml_node<> * root_node;
			size_t i = 0;
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
					m_player->setLivesIcon(livesIcon);
					m_player->setValue(0);
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

								_bullet->onUpdateFunc = [this, soundEngine, _bullet]()
								{
									if (_bullet->getNeedsToBeRemoved())
									{
										if (!_bullet->getExplosionSound().empty())
										{
											soundEngine->play2D(_bullet->getExplosionSound().c_str(), GL_FALSE);
										}

										if (_bullet->getAnimationByIndex("explosion") == nullptr)
										{
											return;
										}

										auto explosion = std::make_shared<BaseGameObject>(_bullet->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
										explosion->applyAnimation(_bullet->getAnimationByIndex("explosion"));
										explosion->onUpdateFunc = [explosion]()
										{
											explosion->changeColor(explosion->getColor().a - 0.05f, 3);

											if (explosion->getColor().a <= 0.0f)
											{
												explosion->setNeedsToBeRemoved(true);
											}
										};
										getCurrentLevel()->addExplosion(explosion);
									}
									else
									{
										auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
										auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

										//Collision detection
										if (_bullet->getPosition().y > windowHeight || (_bullet->getPosition().y + _bullet->getHeight()) < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
										{
											_bullet->setNeedsToBeRemoved(true);
										}
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
	}

	void LevelManager::renderCurrentLevel(float dt, const std::unique_ptr<GameStateManager>& gameStateManager, const std::unique_ptr<InputManager>& inputManager, const std::unique_ptr<CollisionManager>& collisionManager, const std::unique_ptr<Renderer>& renderer, const std::unique_ptr<ConfigurationManager>& configurationManager, const std::unique_ptr<SpriteSheetManager>& spriteSheetManager)
	{
		auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
		auto frameTime = (newTime - m_currentTime) / 1000.0f;
		m_currentTime = newTime;

		m_accumulator += frameTime;

		while (m_accumulator >= dt)
		{
			if (!m_levels.empty())
			{
				bool updateStatus = getCurrentLevel()->update(dt, m_player, gameStateManager, inputManager, collisionManager);

				if (updateStatus && m_levels.size() > 1)
				{
					m_levels.pop();
				}
			}

			m_accumulator -= dt;
		}

		if (!m_levels.empty())
		{
			getCurrentLevel()->render(dt, m_player, gameStateManager, inputManager, collisionManager, renderer, configurationManager, spriteSheetManager);
		}
	}
}