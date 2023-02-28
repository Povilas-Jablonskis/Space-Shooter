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
	const auto playersFileDoc = new rapidxml::xml_document();
	// Read the xml file into a vector
	std::ifstream playersFile(FileConstants::PLAYERS_PATH);
	std::vector playersFileBuffer((std::istreambuf_iterator(playersFile)), std::istreambuf_iterator<char>());
	playersFileBuffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	playersFileDoc->parse<0>(playersFileBuffer.data());
	// Find our root node
	const rapidxml::xml_node<>* playersNode = playersFileDoc->first_node("Players");
	// Iterate over the breweries
	for (auto playerNode = playersNode->first_node("Player"); playerNode; playerNode = playerNode->
	     next_sibling("Player"))
	{
		m_playerModels.emplace_back(playerNode->first_attribute("spriteName")->value());
	}

	playersFile.close();
	playersFileDoc->clear();
	delete playersFileDoc;
}

void PickYourCharacterMenu::onCreate()
{
	//Character Selection
	const auto selectedShip = std::make_shared<Object>(nullptr);
	selectedShip->transform->setPosition(static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)) * 0.47f,
	                                     static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)) * 0.6f);

	const auto selectedShipComponent = selectedShip->addComponent<C_Sprite>();
	selectedShipComponent->setDrawLayer(DrawLayer::UI);

	auto& selectedShipComponentSprite = selectedShipComponent->getSprite();

	selectedShipComponentSprite.setSpriteSheet(m_context.spriteSheet);
	selectedShipComponentSprite.setTextureRect(m_playerModels[m_characterSelectionIndex]);
	selectedShipComponentSprite.setScale(0.5f, 0.5f);

	m_objects.add(selectedShip);

	const auto pickYourCharacterText = std::make_shared<Text>("Pick your character :",
	                                                          glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                          glm::vec2(20.0f, 60.0f), *m_context.font);
	pickYourCharacterText->disable();
	m_texts.push_back(pickYourCharacterText);
	const auto leftArrow = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(44.0f, 60.0f),
	                                              *m_context.font);
	leftArrow->onMouseReleaseFunc = [=, this, &selectedShipComponentSprite]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex -= 1;

		if (m_characterSelectionIndex < 0)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShipComponentSprite.setTextureRect(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(leftArrow);
	const auto rightArrow = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(54.0f, 60.0f),
	                                               *m_context.font);
	rightArrow->onMouseReleaseFunc = [=, this, &selectedShipComponentSprite]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex += 1;

		if (m_characterSelectionIndex > static_cast<int>(m_playerModels.size()) - 1)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShipComponentSprite.setTextureRect(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(rightArrow);

	const auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                               glm::vec2(20.0f, 20.0f),
	                                               *m_context.font);
	backOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	};
	m_texts.push_back(backOption);
	const auto startOption = std::make_shared<Text>("Start", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f),
	                                                glm::vec2(30.0f, 20.0f),
	                                                *m_context.font);
	startOption->onMouseReleaseFunc = [=, this]
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine.remove(ScenesEnum::GAME_LIVE);
		m_sceneStateMachine.add(
			ScenesEnum::GAME_LIVE, std::make_shared<GameScene>(m_sceneStateMachine, m_context, m_characterSelectionIndex));

		m_sceneStateMachine.switchTo(ScenesEnum::GAME_LIVE);
	};
	m_texts.push_back(startOption);
}

void PickYourCharacterMenu::onActivate()
{
	m_context.inputManager->clearEverything();
}

void PickYourCharacterMenu::onDestroy()
{
}

void PickYourCharacterMenu::processInput()
{
	if (m_context.inputManager->isKeyActive(27))
	{
		m_context.soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine.switchTo(ScenesEnum::MAIN);
	}
}

void PickYourCharacterMenu::draw(const float deltaTime)
{
	for (const auto& text : m_texts)
	{
		text->update(*m_context.inputManager);
	}

	m_context.renderer->draw(m_texts);

	m_objects.processCollidingObjects();
	m_objects.processRemovals();
	m_objects.processNewObjects();
	m_objects.update(deltaTime);
	m_objects.draw(*m_context.renderer);
}
