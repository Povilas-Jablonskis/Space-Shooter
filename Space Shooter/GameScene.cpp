#include "GameScene.hpp"
#include "checkCollision.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "FileConstants.hpp"
#include "SpriteSheet.hpp"
#include "SpriteSheetManager.hpp"
#include "Player.hpp"
#include "UIManager.hpp"
#include "ScientificConstants.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"

GameScene::GameScene(std::shared_ptr<SceneStateMachine> sceneStateMachine, std::shared_ptr<SpriteSheetManager> m_spriteSheetManager, std::shared_ptr<InputManager> inputManager, irrklang::ISoundEngine* m_soundEngine, ResourceAllocator<Texture>& textureAllocator)
	: m_sceneStateMachine(sceneStateMachine), m_spriteSheetManager(m_spriteSheetManager), m_inputManager(inputManager), m_soundEngine(m_soundEngine), m_textureAllocator(textureAllocator), m_objects(m_drawbleSystem, m_collisionSystem)
{
	m_context.m_objects = &m_objects;
	m_context.m_background = m_background;
	m_context.m_enemies = &m_enemies;
	m_context.m_explosions = &m_explosions;
	m_context.m_inputManager = m_inputManager;
	m_context.m_meteors = &m_meteors;
	m_context.m_pickups = &m_pickups;
	m_context.m_player = m_player;
	m_context.m_textureAllocator = &m_textureAllocator;
}

void GameScene::loadLevel()
{
	int i = 0;
	auto doc = new rapidxml::xml_document<>();
	rapidxml::xml_node<>* root_node;
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::levelsPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(&buffer[0]);
	// Find our root node
	root_node = doc->first_node("Levels");
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->next_sibling("Level"))
	{
		if (i == m_currentLevel)
		{
			m_background = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(0.0f, 0.0f));
			m_background->applyAnimation(m_spriteSheetManager->getSpriteSheet("background")->getSprite("wholeSpriteSheet"));

			for (auto beer_node = brewery_node->first_node("Enemy"); beer_node; beer_node = beer_node->next_sibling("Enemy"))
			{
				auto positionX = std::stof(beer_node->first_attribute("positionX")->value());
				auto positionY = std::stof(beer_node->first_attribute("positionY")->value());
				auto velocityX = std::stof(beer_node->first_attribute("velocityX")->value());
				auto velocityY = std::stof(beer_node->first_attribute("velocityY")->value());
				std::string spriteName = beer_node->first_attribute("spriteName")->value();

				auto enemy = std::make_shared<Entity>(glm::vec2(positionX, positionY), glm::vec2(velocityX, velocityY), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				enemy->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
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
						bullet->setExplosionSound(explosionSound);
						bullet->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

						for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
						{
							for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
							{
								bullet->addAnimation(beer_node5->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
							}
							for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
							{
								bullet->addAnimation(beer_node5->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
							}
						}

						bullets.push_back(bullet);
					}

					enemy->shootingModeFunc = [=]()
					{
						for (const auto& bullet : bullets)
						{
							auto _bullet = std::make_shared<BaseGameObject>(*bullet);
							auto _bulletPosition = glm::vec2(enemy->getPosition().x + enemy->getWidth() / 2.0f - _bullet->getWidth() / 2.0f, enemy->getPosition().y - enemy->getHeight() / 2.0f);
							_bulletPosition += glm::vec2(enemy->getWidth() * _bullet->getPosition().x, enemy->getHeight() * _bullet->getPosition().y);
							_bullet->setPosition(_bulletPosition);

							_bullet->onUpdateFunc = [=]()
							{
								if (_bullet->getNeedsToBeRemoved() && _bullet->getLives() <= 1)
								{
									if (!_bullet->getExplosionSound().empty())
									{
										m_soundEngine->play2D(_bullet->getExplosionSound().c_str(), GL_FALSE);
									}

									if (_bullet->getAnimationByIndex("explosion") == nullptr)
									{
										return;
									}

									auto explosion = std::make_shared<BaseGameObject>(_bullet->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
									explosion->applyAnimation(_bullet->getAnimationByIndex("explosion"));
									explosion->onUpdateFunc = [=]()
									{
										explosion->changeColor(explosion->getColor().a - 0.05f, 3);

										if (explosion->getColor().a <= 0.0f)
										{
											explosion->setNeedsToBeRemoved(true);
										}
									};
									m_explosions.push_back(explosion);
								}
								else
								{
									const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
									const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

									//Collision detection
									if (_bullet->getPosition().y > windowHeight || _bullet->getPosition().y + _bullet->getHeight() < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
									{
										_bullet->setNeedsToBeRemoved(true);
									}
								}
							};

							enemy->addBullet(_bullet);
						}

						m_soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
					};
					enemy->setDelayBetweenShoots(delayBetweenShoots);
				}

				for (auto beer_node2 = beer_node->first_node("Animations"); beer_node2; beer_node2 = beer_node2->next_sibling("Animations"))
				{
					for (auto beer_node3 = beer_node2->first_node("Sprite"); beer_node3; beer_node3 = beer_node3->next_sibling("Sprite"))
					{
						enemy->addAnimation(beer_node3->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));
					}

					for (auto beer_node3 = beer_node2->first_node("Animation"); beer_node3; beer_node3 = beer_node3->next_sibling("Animation"))
					{
						enemy->addAnimation(beer_node3->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node3->first_attribute("animationName")->value()));
					}
				}

				enemy->onUpdateFunc = [=]()
				{
					const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
					const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

					//Collision detection
					if (enemy->getPosition().x + enemy->getWidth() >= windowWidth || enemy->getPosition().x <= 0.0f)
						enemy->setVelocity(0, enemy->getVelocity().x * -1.0f);

					if (enemy->getPosition().y + enemy->getHeight() >= windowHeight || enemy->getPosition().y <= 0.0f)
						enemy->setVelocity(1, enemy->getVelocity().y * -1.0f);
				};

				m_enemies.push_back(enemy);
			}

			for (auto beer_node = brewery_node->first_node("Meteor"); beer_node; beer_node = beer_node->next_sibling("Meteor"))
			{
				auto meteor = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				meteor->setExplosionSound(beer_node->first_attribute("explosionSound")->value());
				meteor->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
				meteor->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node->first_attribute("spriteName")->value()));
				meteor->onUpdateFunc = [=]()
				{
					meteor->setRotationAngle(meteor->getRotationAngle() + 0.05f);

					if (meteor->getRotationAngle() >= 3.14159265358979323846f * 2.0f)
					{
						meteor->setRotationAngle(0.0f);
					}
				};

				meteor->setValue(100);
				m_meteors.push_back(meteor);
			}

			for (auto beer_node = brewery_node->first_node("Pickup"); beer_node; beer_node = beer_node->next_sibling("Pickup"))
			{
				std::string spriteName = beer_node->first_attribute("spriteName")->value();
				auto pickup = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()), std::stof(beer_node->first_attribute("positionY")->value())), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
				pickup->setExplosionSound(beer_node->first_attribute("pickupSound")->value());
				pickup->setVelocity(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()), std::stof(beer_node->first_attribute("velocityY")->value())));
				pickup->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));

				for (auto beer_node2 = beer_node->first_node("ShootingEffect"); beer_node2; beer_node2 = beer_node2->next_sibling("ShootingEffect"))
				{
					std::string explosionSound = beer_node2->first_attribute("explosionSound")->value();
					std::vector<std::shared_ptr<BaseGameObject>> bullets;
					auto delayBetweenShoots = std::stof(beer_node2->first_attribute("delayBetweenShoots")->value());
					std::string shootingSound = beer_node2->first_attribute("shootingSound")->value();

					for (auto beer_node3 = beer_node2->first_node("Bullet"); beer_node3; beer_node3 = beer_node3->next_sibling("Bullet"))
					{
						auto bullet = std::make_shared<BaseGameObject>(glm::vec2(std::stof(beer_node3->first_attribute("offsetX")->value()), std::stof(beer_node3->first_attribute("offsetY")->value())), glm::vec2(std::stof(beer_node3->first_attribute("velocityX")->value()), std::stof(beer_node3->first_attribute("velocityY")->value())), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
						bullet->setExplosionSound(explosionSound);
						bullet->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node3->first_attribute("spriteName")->value()));

						for (auto beer_node4 = beer_node3->first_node("Animations"); beer_node4; beer_node4 = beer_node4->next_sibling("Animations"))
						{
							for (auto beer_node5 = beer_node4->first_node("Sprite"); beer_node5; beer_node5 = beer_node5->next_sibling("Sprite"))
							{
								bullet->addAnimation(beer_node5->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node5->first_attribute("spriteName")->value()));
							}
							for (auto beer_node5 = beer_node4->first_node("Animation"); beer_node5; beer_node5 = beer_node5->next_sibling("Animation"))
							{
								bullet->addAnimation(beer_node5->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node5->first_attribute("animationName")->value()));
							}
						}

						bullets.push_back(bullet);
					}

					pickup->onCollisionFunc = [=](const std::shared_ptr<BaseGameObject>& collider)
					{
						auto entity = dynamic_cast<Entity*>(collider.get());

						pickup->setNeedsToBeRemoved(true);

						entity->shootingModeFunc = [=]()
						{
							for (const auto& bullet : bullets)
							{
								auto _bullet = std::make_shared<BaseGameObject>(*bullet);
								auto _bulletPosition = glm::vec2(entity->getPosition().x + entity->getWidth() / 2.0f - _bullet->getWidth() / 2.0f, entity->getPosition().y + entity->getHeight() / 2.0f);
								_bulletPosition += glm::vec2(entity->getWidth() * _bullet->getPosition().x, entity->getHeight() * _bullet->getPosition().y);
								_bullet->setPosition(_bulletPosition);

								_bullet->onUpdateFunc = [=]()
								{
									if (_bullet->getNeedsToBeRemoved() && _bullet->getLives() <= 1)
									{
										if (!_bullet->getExplosionSound().empty())
										{
											m_soundEngine->play2D(_bullet->getExplosionSound().c_str(), GL_FALSE);
										}

										if (_bullet->getAnimationByIndex("explosion") == nullptr)
										{
											return;
										}

										auto explosion = std::make_shared<BaseGameObject>(_bullet->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
										explosion->applyAnimation(_bullet->getAnimationByIndex("explosion"));
										explosion->onUpdateFunc = [=]()
										{
											explosion->changeColor(explosion->getColor().a - 0.05f, 3);

											if (explosion->getColor().a <= 0.0f)
											{
												explosion->setNeedsToBeRemoved(true);
											}
										};
										m_explosions.push_back(explosion);
									}
									else
									{
										const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
										const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

										//Collision detection
										if (_bullet->getPosition().y > windowHeight || _bullet->getPosition().y + _bullet->getHeight() < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
										{
											_bullet->setNeedsToBeRemoved(true);
										}
									}
								};

								entity->addBullet(_bullet);
							}

							m_soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
						};

						entity->setDelayBetweenShoots(delayBetweenShoots);
						entity->setDelayBetweenShootsTimer(0.0f);
						return true;
					};
				}

				for (auto beer_node2 = beer_node->first_node("Shield"); beer_node2; beer_node2 = beer_node2->next_sibling("Shield"))
				{
					std::string shieldSpriteName = beer_node2->first_attribute("spriteName")->value();

					pickup->onCollisionFunc = [=](const std::shared_ptr<BaseGameObject>& collider)
					{
						auto entity = dynamic_cast<Entity*>(collider.get());

						pickup->setNeedsToBeRemoved(true);

						auto shield = std::make_shared<BaseGameObject>(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 0.7f));

						shield->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(shieldSpriteName));
						shield->onUpdateFunc = [=]()
						{
							shield->changeColor(shield->getColor().a - 0.05f, 3);

							if (shield->getColor().a <= 0.0f)
							{
								shield->changeColor(0.7f, 3);
							}

							shield->setPosition(entity->getPosition() + glm::vec2((shield->getWidth() - entity->getWidth()) * -1.0f / 2.0f, (shield->getHeight() - entity->getHeight()) * -1.0f / 2.0f));
						};
						entity->addAddon("shield", shield);
						return true;
					};
				}

				m_pickups.push_back(pickup);
			}

			break;
		}
		i++;
	}

	theFile.close();
	doc->clear();
	delete doc;
}

void GameScene::onCreate()
{
	{
		auto doc = new rapidxml::xml_document<>();
		rapidxml::xml_node<>* root_node;
		// Read the xml file into a vector
		std::ifstream theFile(FileConstants::levelsPath);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc->first_node("Levels");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->next_sibling("Level"))
		{
			m_maxLevels++;
		}
	}

	{
		auto doc = new rapidxml::xml_document<>();
		rapidxml::xml_node<>* root_node;
		int i = 0;
		// Read the xml file into a vector
		std::ifstream theFile(FileConstants::playersPath);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(&buffer[0]);
		// Find our root node
		root_node = doc->first_node("Players");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->next_sibling("Player"))
		{
			if (i == 0)
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
				m_player->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));

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
						bullet->applyAnimation(m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));

						for (auto beer_node3 = beer_node2->first_node("Animations"); beer_node3; beer_node3 = beer_node3->next_sibling("Animations"))
						{
							for (auto beer_node4 = beer_node3->first_node("Sprite"); beer_node4; beer_node4 = beer_node4->next_sibling("Sprite"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node4->first_attribute("spriteName")->value()));
							}
							for (auto beer_node4 = beer_node3->first_node("Animation"); beer_node4; beer_node4 = beer_node4->next_sibling("Animation"))
							{
								bullet->addAnimation(beer_node4->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node4->first_attribute("animationName")->value()));
							}
						}

						bullets.push_back(bullet);
					}

					m_player->shootingModeFunc = [=]()
					{
						for (const auto& bullet : bullets)
						{
							auto _bullet = std::make_shared<BaseGameObject>(*bullet);
							auto _bulletPosition = glm::vec2(m_player->getPosition().x + m_player->getWidth() / 2.0f - _bullet->getWidth() / 2.0f, m_player->getPosition().y + m_player->getHeight() / 2.0f);
							_bulletPosition += glm::vec2(m_player->getWidth() * _bullet->getPosition().x, m_player->getHeight() * _bullet->getPosition().y);
							_bullet->setPosition(_bulletPosition);

							_bullet->onUpdateFunc = [=]()
							{
								if (_bullet->getNeedsToBeRemoved())
								{
									if (!_bullet->getExplosionSound().empty())
									{
										m_soundEngine->play2D(_bullet->getExplosionSound().c_str(), GL_FALSE);
									}

									if (_bullet->getAnimationByIndex("explosion") == nullptr)
									{
										return;
									}

									auto explosion = std::make_shared<BaseGameObject>(_bullet->getPosition(), glm::vec2(0.0f, 0.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
									explosion->applyAnimation(_bullet->getAnimationByIndex("explosion"));
									explosion->onUpdateFunc = [=]()
									{
										explosion->changeColor(explosion->getColor().a - 0.05f, 3);

										if (explosion->getColor().a <= 0.0f)
										{
											explosion->setNeedsToBeRemoved(true);
										}
									};
									m_explosions.push_back(explosion);
								}
								else
								{
									const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
									const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

									//Collision detection
									if (_bullet->getPosition().y > windowHeight || _bullet->getPosition().y + _bullet->getHeight() < 0.0f || _bullet->getPosition().x > windowWidth || _bullet->getPosition().x < 0.0f)
									{
										_bullet->setNeedsToBeRemoved(true);
									}
								}
							};

							m_player->addBullet(_bullet);
						}

						m_soundEngine->play2D(shootingSound.c_str(), GL_FALSE);
					};
					m_player->setDelayBetweenShoots(delayBetweenShoots);
				}

				for (auto beer_node = brewery_node->first_node("Animations"); beer_node; beer_node = beer_node->next_sibling("Animations"))
				{
					for (auto beer_node2 = beer_node->first_node("Sprite"); beer_node2; beer_node2 = beer_node2->next_sibling("Sprite"))
					{
						m_player->addAnimation(beer_node2->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getSprite(beer_node2->first_attribute("spriteName")->value()));
					}

					for (auto beer_node2 = beer_node->first_node("Animation"); beer_node2; beer_node2 = beer_node2->next_sibling("Animation"))
					{
						m_player->addAnimation(beer_node2->first_attribute("name")->value(), m_spriteSheetManager->getSpriteSheet("main")->getAnimation(beer_node2->first_attribute("animationName")->value()));
					}
				}

				break;
			}

			++i;
		}

		doc->clear();
		delete doc;
	}

	loadLevel();
}

void GameScene::onActivate()
{
	m_accumulator = 0;
	m_currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	m_inputManager->clearEverything();
}

void GameScene::onDestroy() { }

void GameScene::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::PAUSED);
	}
}

void GameScene::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	const auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	const auto frameTime = (newTime - m_currentTime) / 1000.0f;
	m_currentTime = newTime;

	m_accumulator += frameTime;

	while (m_accumulator >= dt)
	{
		auto lastPlayerAlive = m_player->getNeedsToBeRemoved();
		auto lastEnemiesEmpty = m_enemies.empty();

		m_player->update(dt, m_inputManager);

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

		for (auto& enemy : m_enemies)
		{
			checkCollision(m_player, enemy->getBulletsList());
			checkCollision(enemy, m_player->getBulletsList());
		}

		for (auto& m_meteor : m_meteors)
		{
			checkCollision(m_meteor, m_player->getBulletsList());
		}

		checkCollision(m_player, &m_meteors);
		checkCollision(m_player, &m_enemies);
		checkCollision(m_player, &m_pickups);

		if (lastPlayerAlive == false && m_player->getNeedsToBeRemoved())
		{
			m_sceneStateMachine->switchTo(ScenesEnum::GAME_OVER);
		}
		else if (lastEnemiesEmpty == false && m_enemies.empty())
		{
			m_currentLevel++;

			if (m_currentLevel < m_maxLevels)
			{
				loadLevel();
			}
			else
			{
				m_sceneStateMachine->switchTo(ScenesEnum::GAME_WON);
			}
		}

		m_objects.processRemovals();
		m_objects.processNewObjects();
		m_objects.update(dt);

		m_accumulator -= dt;
	}

	//Render m_background
	m_background->update(dt, m_inputManager);
	renderer->draw(m_background);
	m_objects.draw(renderer);
	//Render m_meteors
	renderer->draw(m_meteors);
	//Render m_explosions
	renderer->draw(m_explosions);
	//Render m_player & his addons
	renderer->draw(m_player);
	for (auto& addon : *m_player->getAddons())
	{
		renderer->draw(addon.second);
	}
	//Render m_enemies & their addons
	renderer->draw(m_enemies);
	for (auto& enemy : m_enemies)
	{
		for (auto& it2 : *enemy->getAddons())
		{
			renderer->draw(it2.second);
		}
	}
	//Render m_pickups
	renderer->draw(m_pickups);
	//Render m_enemies bullets
	for (auto& enemy : m_enemies)
	{
		renderer->draw(*enemy->getBulletsList());
	}
	//Render m_player's bullets
	renderer->draw(*m_player->getBulletsList());
	//Render & Update UI elements
	m_uiManager->updateUI(m_player->getValue(), m_player->getLivesIcon(), m_player->getLives(), dt, m_inputManager, m_spriteSheetManager);

	m_uiManager->renderUI(renderer);
}