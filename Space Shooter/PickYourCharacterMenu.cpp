#include "PickYourCharacterMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "FileConstants.hpp"
#include "GameScene.hpp"
#include "InputManager.hpp"
#include "C_Transform.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"
#include <glew/glew.h>
#include <freeglut/freeglut.h>

PickYourCharacterMenu::PickYourCharacterMenu(SceneStateMachine& sceneStateMachine, SharedContext& context)
	: m_context(context), m_sceneStateMachine(sceneStateMachine)
{
}

void PickYourCharacterMenu::loadPlayerModels()
{
	const auto doc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::playersPath);
	std::vector buffer((std::istreambuf_iterator(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(buffer.data());
	// Find our root node
	const rapidxml::xml_node<>* root_node = doc->first_node("Players");
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->
	     next_sibling("Player"))
	{
		m_playerModels.emplace_back(brewery_node->first_attribute("spriteName")->value());
	}

	theFile.close();
	doc->clear();
	delete doc;
}

void PickYourCharacterMenu::onCreate()
{
	//Character Selection
	const auto selectedShip = std::make_shared<Object>(nullptr);
	selectedShip->m_transform->setPosition(static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)) * 0.47f,
	                                       static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) * 0.6f);

	const auto selectedShipSprite = selectedShip->addComponent<C_Sprite>();
	selectedShipSprite->setDrawLayer(DrawLayer::UI);

	selectedShipSprite->getSprite().setSpriteSheet(m_context.m_spriteSheet);
	selectedShipSprite->getSprite().setTextureRect(m_playerModels[m_characterSelectionIndex]);
	selectedShipSprite->getSprite().setScale(0.5f, 0.5f);

	m_objects.add(selectedShip);

	const auto pickYourCharacterText = std::make_shared<Text>("Pick your character :",
	                                                          glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                          glm::vec2(20.0f, 60.0f), *m_context.m_font);
	pickYourCharacterText->disable();
	m_texts.push_back(pickYourCharacterText);
	const auto leftArrow = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(44.0f, 60.0f),
	                                              *m_context.m_font);
	leftArrow->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex -= 1;

		if (m_characterSelectionIndex < 0)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShipSprite->getSprite().setTextureRect(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(leftArrow);
	const auto rightArrow = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(54.0f, 60.0f),
	                                               *m_context.m_font);
	rightArrow->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex += 1;

		if (m_characterSelectionIndex > static_cast<int>(m_playerModels.size()) - 1)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShipSprite->getSprite().setTextureRect(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(rightArrow);

	const auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(20.0f, 20.0f),
	                                               *m_context.m_font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(MAIN);
	};
	m_texts.push_back(backOption);
	const auto startOption = std::make_shared<Text>("Start", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                glm::vec2(30.0f, 20.0f),
	                                                *m_context.m_font);
	startOption->onMouseReleaseFunc = [=, this]
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.remove(GAME_LIVE);
		m_sceneStateMachine.add(
			GAME_LIVE, std::make_shared<GameScene>(m_sceneStateMachine, m_context, m_characterSelectionIndex));

		m_sceneStateMachine.switchTo(GAME_LIVE);
	};
	m_texts.push_back(startOption);
}

void PickYourCharacterMenu::onActivate()
{
	m_context.m_inputManager->clearEverything();
}

void PickYourCharacterMenu::onDestroy()
{
}

void PickYourCharacterMenu::processInput()
{
	if (m_context.m_inputManager->getKey(27))
	{
		m_context.m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(MAIN);
	}
}

void PickYourCharacterMenu::draw(const float dt)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.m_inputManager);
	}

	m_context.m_renderer->draw(m_texts);

	m_objects.processCollidingObjects();
	m_objects.processRemovals();
	m_objects.processNewObjects();
	m_objects.update(dt);
	m_objects.draw(*m_context.m_renderer);
}
