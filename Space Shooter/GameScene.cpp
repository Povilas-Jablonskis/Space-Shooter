#include "GameScene.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "FileConstants.hpp"
#include "Sprite.hpp"
#include "Object.hpp"
#include "SharedContext.hpp"
#include "InputManager.hpp"
#include "ObjectCollection.hpp"
#include "C_Transform.hpp"
#include "C_KeyboardMovement.hpp"
#include "C_Velocity_ReverseVelocityWhenOutOfBounds.hpp"
#include "C_Velocity_DestroyWhenOutOfBounds.hpp"
#include "C_PlayerLivesAndScore.hpp"
#include "C_BoxCollider.hpp"
#include "C_Tag.hpp"
#include "C_RemoveObjectOnCollisionEnter.hpp"

#include <fstream>

#include "rapidxml/rapidxml_print.hpp"

GameScene::GameScene(SceneStateMachine& sceneStateMachine, SharedContext& context, const int characterSelectionIndex)
	: m_characterSelectionIndex(characterSelectionIndex), m_context(context), m_sceneStateMachine(sceneStateMachine)
{
	m_context.m_objects = &m_objects;
}

void GameScene::loadLevel()
{
	int i = 0;
	const auto doc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::levelsPath);
	std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(buffer.data());
	// Find our root node
	const rapidxml::xml_node<>* root_node = doc->first_node("Levels");
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->
	     next_sibling("Level"))
	{
		if (i == m_currentLevel)
		{
			const auto background = std::make_shared<Object>(&m_context);

			const auto sprite = background->addComponent<C_Sprite>();
			sprite->setDrawLayer(DrawLayer::Background);

			sprite->getSprite().setSpriteSheet(&m_spriteSheet);
			sprite->getSprite().setTextureRect("wholeSpriteSheet");
			sprite->getSprite().setScale(4.0f, 3.0f);

			m_objects.add(background);

			for (auto beer_node = brewery_node->first_node("Enemy"); beer_node; beer_node = beer_node->
			     next_sibling("Enemy"))
			{
				auto enemy = std::make_shared<Object>(&m_context);
				enemy->m_tag->set(Tag::NPC);
				enemy->m_transform->setPosition(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()),
				                                          std::stof(beer_node->first_attribute("positionY")->value())));

				const auto enemySprite = enemy->addComponent<C_Sprite>();
				enemySprite->setDrawLayer(DrawLayer::Entities);

				enemySprite->getSprite().setSpriteSheet(m_context.m_spriteSheet);
				enemySprite->getSprite().setTextureRect(beer_node->first_attribute("spriteName")->value());
				enemySprite->getSprite().setScale(0.5f, 0.5f);

				const auto collider = enemy->addComponent<C_BoxCollider>();
				collider->setSize(enemySprite->getSprite().getTextureRect().z * enemySprite->getSprite().getScale().x,
				                  enemySprite->getSprite().getTextureRect().w * enemySprite->getSprite().getScale().y);
				collider->setLayer(CollisionLayer::Enemy);

				const auto velocity = enemy->addComponent<C_Velocity_ReverseVelocityWhenOutOfBounds>();
				velocity->set(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()),
				                        std::stof(beer_node->first_attribute("velocityY")->value())));

				m_objects.add(enemy);
			}

			for (auto beer_node = brewery_node->first_node("Meteor"); beer_node; beer_node = beer_node->
			     next_sibling("Meteor"))
			{
				auto meteor = std::make_shared<Object>(&m_context);
				meteor->m_tag->set(Tag::NPC);
				meteor->m_transform->setPosition(glm::vec2(std::stof(beer_node->first_attribute("positionX")->value()),
				                                           std::stof(beer_node->first_attribute(
					                                           "positionY")->value())));

				const auto meteorSprite = meteor->addComponent<C_Sprite>();
				meteorSprite->setDrawLayer(DrawLayer::Entities);

				meteorSprite->getSprite().setSpriteSheet(m_context.m_spriteSheet);
				meteorSprite->getSprite().setTextureRect(beer_node->first_attribute("spriteName")->value());
				meteorSprite->getSprite().setScale(0.5f, 0.5f);

				const auto collider = meteor->addComponent<C_BoxCollider>();
				collider->setSize(meteorSprite->getSprite().getTextureRect().z * meteorSprite->getSprite().getScale().x,
				                  meteorSprite->getSprite().getTextureRect().w * meteorSprite->getSprite().getScale().
				                  y);
				collider->setLayer(CollisionLayer::Meteor);

				const auto velocity = meteor->addComponent<C_Velocity_DestroyWhenOutOfBounds>();
				velocity->set(glm::vec2(std::stof(beer_node->first_attribute("velocityX")->value()),
				                        std::stof(beer_node->first_attribute("velocityY")->value())));

				meteor->addComponent<C_RemoveObjectOnCollisionEnter>();

				m_objects.add(meteor);
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
		auto doc = new rapidxml::xml_document();
		rapidxml::xml_node<>* root_node;
		// Read the xml file into a vector
		std::ifstream theFile(FileConstants::levelsPath);
		std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(buffer.data());
		// Find our root node
		root_node = doc->first_node("Levels");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Level"); brewery_node; brewery_node = brewery_node->
		     next_sibling("Level"))
		{
			m_maxLevels++;
		}
	}

	{
		auto doc = new rapidxml::xml_document();
		rapidxml::xml_node<>* root_node;
		int i = 0;
		// Read the xml file into a vector
		std::ifstream theFile(FileConstants::playersPath);
		std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc->parse<0>(buffer.data());
		// Find our root node
		root_node = doc->first_node("Players");
		// Iterate over the breweries
		for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->
		     next_sibling("Player"))
		{
			if (i == m_characterSelectionIndex)
			{
				auto player = std::make_shared<Object>(&m_context);
				player->m_tag->set(Tag::Player);
				player->m_transform->setPosition(static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)) / 2.0f, 0.0f);

				auto sprite = player->addComponent<C_Sprite>();
				sprite->setDrawLayer(DrawLayer::Entities);

				player->addComponent<C_KeyboardMovement>();

				sprite->getSprite().setSpriteSheet(m_context.m_spriteSheet);
				sprite->getSprite().setTextureRect(brewery_node->first_attribute("spriteName")->value());
				sprite->getSprite().setScale(0.5f, 0.5f);

				auto collider = player->addComponent<C_BoxCollider>();
				collider->setSize(sprite->getSprite().getTextureRect().z * sprite->getSprite().getScale().x,
				                  sprite->getSprite().getTextureRect().w * sprite->getSprite().getScale().y);
				collider->setLayer(CollisionLayer::Player);

				player->addComponent<C_Velocity_ReverseVelocityWhenOutOfBounds>();
				auto playerLivesAndScore = player->addComponent<C_PlayerLivesAndScore>();
				playerLivesAndScore->setLivesIcon(brewery_node->first_attribute("livesIcon")->value());

				m_objects.add(player);

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
	m_context.m_inputManager->clearEverything();
}

void GameScene::onDestroy()
{
}

void GameScene::processInput()
{
	if (m_context.m_inputManager->getKey(27))
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(PAUSED);
	}
}

void GameScene::draw(const float dt)
{
	const auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	const auto frameTime = (newTime - m_currentTime) / 1000.0f;
	m_currentTime = newTime;

	m_accumulator += frameTime;

	while (m_accumulator >= dt)
	{
		m_objects.processCollidingObjects();
		m_objects.processRemovals();
		m_objects.processNewObjects();
		m_objects.update(dt);

		m_accumulator -= dt;
	}

	m_objects.draw(*m_context.m_renderer);
}
