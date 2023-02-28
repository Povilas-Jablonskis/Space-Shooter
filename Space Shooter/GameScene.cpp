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
	m_context.objects = &m_objects;
}

void GameScene::loadLevel()
{
	int index = 0;
	const auto levelsFileDoc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream levelsFile(FileConstants::LEVELS_PATH);
	std::vector levelsFileBuffer((std::istreambuf_iterator(levelsFile)), std::istreambuf_iterator<char>());
	levelsFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	levelsFileDoc->parse<0>(levelsFileBuffer.data());
	// Find our root node
	const rapidxml::xml_node<>* levelsNode = levelsFileDoc->first_node("Levels");
	// Iterate over the breweries
	for (auto levelNode = levelsNode->first_node("Level"); levelNode; levelNode = levelNode->
	     next_sibling("Level"))
	{
		if (index == m_currentLevel)
		{
			const auto background = std::make_shared<Object>(&m_context);

			const auto backgroundComponent = background->addComponent<C_Sprite>();
			backgroundComponent->setDrawLayer(DrawLayer::BACKGROUND);

			auto& backgroundComponentSprite = backgroundComponent->getSprite();

			backgroundComponentSprite.setSpriteSheet(&m_spriteSheet);
			backgroundComponentSprite.setTextureRect("wholeSpriteSheet");
			backgroundComponentSprite.setScale(4.0f, 3.0f);

			m_objects.add(background);

			for (auto enemyNode = levelNode->first_node("Enemy"); enemyNode; enemyNode = enemyNode->
			     next_sibling("Enemy"))
			{
				auto enemy = std::make_shared<Object>(&m_context);
				enemy->tag->set(Tag::NPC);
				enemy->transform->setPosition(glm::vec2(std::stof(enemyNode->first_attribute("positionX")->value()),
				                                        std::stof(enemyNode->first_attribute("positionY")->value())));

				const auto enemyComponent = enemy->addComponent<C_Sprite>();
				enemyComponent->setDrawLayer(DrawLayer::ENTITIES);

				auto& enemyComponentSprite = enemyComponent->getSprite();

				enemyComponentSprite.setSpriteSheet(m_context.spriteSheet);
				enemyComponentSprite.setTextureRect(enemyNode->first_attribute("spriteName")->value());
				enemyComponentSprite.setScale(0.5f, 0.5f);

				const auto enemyComponentSpriteRect = enemyComponentSprite.getTextureRect();
				const auto enemyComponentSpriteScale = enemyComponentSprite.getScale();

				const auto collider = enemy->addComponent<C_BoxCollider>();
				collider->setSize(enemyComponentSpriteRect.z * enemyComponentSpriteScale.x,
				                  enemyComponentSpriteRect.w * enemyComponentSpriteScale.y);
				collider->setLayer(CollisionLayer::ENEMY);

				const auto velocity = enemy->addComponent<C_Velocity_ReverseVelocityWhenOutOfBounds>();
				velocity->set(glm::vec2(std::stof(enemyNode->first_attribute("velocityX")->value()),
				                        std::stof(enemyNode->first_attribute("velocityY")->value())));

				m_objects.add(enemy);
			}

			for (auto meteorNode = levelNode->first_node("Meteor"); meteorNode; meteorNode = meteorNode->
			     next_sibling("Meteor"))
			{
				auto meteor = std::make_shared<Object>(&m_context);
				meteor->tag->set(Tag::NPC);
				meteor->transform->setPosition(glm::vec2(std::stof(meteorNode->first_attribute("positionX")->value()),
				                                         std::stof(meteorNode->first_attribute(
					                                         "positionY")->value())));

				const auto meteorComponent = meteor->addComponent<C_Sprite>();
				meteorComponent->setDrawLayer(DrawLayer::ENTITIES);

				auto& meteorComponentSprite = meteorComponent->getSprite();

				meteorComponentSprite.setSpriteSheet(m_context.spriteSheet);
				meteorComponentSprite.setTextureRect(meteorNode->first_attribute("spriteName")->value());
				meteorComponentSprite.setScale(0.5f, 0.5f);

				const auto meteorComponentSpriteRect = meteorComponentSprite.getTextureRect();
				const auto meteorComponentSpriteScale = meteorComponentSprite.getScale();

				const auto collider = meteor->addComponent<C_BoxCollider>();
				collider->setSize(meteorComponentSpriteRect.z * meteorComponentSpriteScale.x,
				                  meteorComponentSpriteRect.w * meteorComponentSpriteScale.
				                  y);
				collider->setLayer(CollisionLayer::METEOR);

				const auto velocity = meteor->addComponent<C_Velocity_DestroyWhenOutOfBounds>();
				velocity->set(glm::vec2(std::stof(meteorNode->first_attribute("velocityX")->value()),
				                        std::stof(meteorNode->first_attribute("velocityY")->value())));

				meteor->addComponent<C_RemoveObjectOnCollisionEnter>();

				m_objects.add(meteor);
			}

			break;
		}
		index++;
	}

	levelsFile.close();
	levelsFileDoc->clear();
	delete levelsFileDoc;
}

void GameScene::onCreate()
{
	{
		auto levelsFileDoc = new rapidxml::xml_document();
		// Read the xml file into a vector
		std::ifstream levelsFile(FileConstants::LEVELS_PATH);
		std::vector levelsFileStream((std::istreambuf_iterator(levelsFile)), std::istreambuf_iterator<char>());
		levelsFileStream.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		levelsFileDoc->parse<0>(levelsFileStream.data());
		// Find our root node
		rapidxml::xml_node<>* levelsNode = levelsFileDoc->first_node("Levels");
		// Iterate over the breweries
		for (auto levelNode = levelsNode->first_node("Level"); levelNode; levelNode = levelNode->
		     next_sibling("Level"))
		{
			m_maxLevels++;
		}
	}

	{
		auto playersFileDoc = new rapidxml::xml_document();
		int index = 0;
		// Read the xml file into a vector
		std::ifstream playersFile(FileConstants::PLAYERS_PATH);
		std::vector playersFileStream((std::istreambuf_iterator(playersFile)), std::istreambuf_iterator<char>());
		playersFileStream.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		playersFileDoc->parse<0>(playersFileStream.data());
		// Find our root node
		rapidxml::xml_node<>* playersNode = playersFileDoc->first_node("Players");
		// Iterate over the breweries
		for (auto playerNode = playersNode->first_node("Player"); playerNode; playerNode = playerNode->
		     next_sibling("Player"))
		{
			if (index == m_characterSelectionIndex)
			{
				auto player = std::make_shared<Object>(&m_context);
				player->tag->set(Tag::PLAYER);
				player->transform->setPosition(static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)) / 2.0f, 0.0f);

				auto playerComponent = player->addComponent<C_Sprite>();
				playerComponent->setDrawLayer(DrawLayer::ENTITIES);

				auto& playerComponentSprite = playerComponent->getSprite();

				player->addComponent<C_KeyboardMovement>();

				playerComponentSprite.setSpriteSheet(m_context.spriteSheet);
				playerComponentSprite.setTextureRect(playerNode->first_attribute("spriteName")->value());
				playerComponentSprite.setScale(0.5f, 0.5f);

				const auto playerComponentSpriteRect = playerComponentSprite.getTextureRect();
				const auto playerComponentSpriteScale = playerComponentSprite.getScale();


				auto collider = player->addComponent<C_BoxCollider>();
				collider->setSize(playerComponentSpriteRect.z * playerComponentSpriteScale.x,
				                  playerComponentSpriteRect.w * playerComponentSpriteScale.y);
				collider->setLayer(CollisionLayer::PLAYER);

				player->addComponent<C_Velocity_ReverseVelocityWhenOutOfBounds>();
				auto playerLivesAndScore = player->addComponent<C_PlayerLivesAndScore>();
				playerLivesAndScore->setLivesIcon(playerNode->first_attribute("livesIcon")->value());

				m_objects.add(player);

				break;
			}

			++index;
		}

		playersFileDoc->clear();
		delete playersFileDoc;
	}

	loadLevel();
}

void GameScene::onActivate()
{
	m_accumulator = 0;
	m_currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	m_context.inputManager->clearEverything();
}

void GameScene::onDestroy()
{
}

void GameScene::processInput()
{
	if (m_context.inputManager->isKeyActive(27))
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::PAUSED);
	}
}

void GameScene::draw(const float deltaTime)
{
	const auto newTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	const auto frameTime = (newTime - m_currentTime) / 1000.0f;
	m_currentTime = newTime;

	m_accumulator += frameTime;

	while (m_accumulator >= deltaTime)
	{
		m_objects.processCollidingObjects();
		m_objects.processRemovals();
		m_objects.processNewObjects();
		m_objects.update(deltaTime);

		m_accumulator -= deltaTime;
	}

	m_objects.draw(*m_context.renderer);
}
