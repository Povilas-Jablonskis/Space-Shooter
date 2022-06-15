#include "PickYourCharacterMenu.hpp"
#include "SceneStateMachine.hpp"
#include "Text.hpp"
#include "Renderer.hpp"
#include "SpriteSheet.hpp"
#include "SpriteSheetManager.hpp"
#include "FileConstants.hpp"
#include "GameScene.hpp"

#include <fstream>
#include "rapidxml/rapidxml_print.hpp"


PickYourCharacterMenu::PickYourCharacterMenu(std::shared_ptr<SceneStateMachine> sceneStateMachine, std::shared_ptr<SpriteSheetManager> m_spriteSheetManager, std::shared_ptr<InputManager> inputManager, irrklang::ISoundEngine* m_soundEngine)
	: m_sceneStateMachine(sceneStateMachine), m_spriteSheetManager(m_spriteSheetManager), m_inputManager(inputManager), m_soundEngine(m_soundEngine)
{

}

void PickYourCharacterMenu::loadPlayerModels(const std::shared_ptr<SpriteSheetManager>& spriteSheetManager)
{
	auto doc = new rapidxml::xml_document<>();
	// Read the xml file into a vector
	std::ifstream theFile(FileConstants::playersPath);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc->parse<0>(&buffer[0]);
	// Find our root node
	rapidxml::xml_node<>* root_node = doc->first_node("Players");
	// Iterate over the breweries
	for (auto brewery_node = root_node->first_node("Player"); brewery_node; brewery_node = brewery_node->next_sibling("Player"))
	{
		std::string spriteName = brewery_node->first_attribute("spriteName")->value();

		m_playerModels.push_back(spriteSheetManager->getSpriteSheet("main")->getSprite(spriteName));
	}

	theFile.close();
	doc->clear();
	delete doc;
}

void PickYourCharacterMenu::onCreate()
{
	//Character Selection
	auto selectedShip = std::make_shared<UIElementBase>(glm::vec4(255.0f, 255.0f, 255.0f, 1.0f), glm::vec2(47.0f, 60.0f));
	selectedShip->applyAnimation(m_playerModels[m_characterSelectionIndex]);
	selectedShip->setScale(0.5f);
	m_elements.push_back(selectedShip);

	auto pickYourCharacterText = std::make_shared<Text>("Pick your character :", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 60.0f));
	pickYourCharacterText->disable();
	m_texts.push_back(pickYourCharacterText);
	auto leftArrow = std::make_shared<Text>("<", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(44.0f, 60.0f));
	leftArrow->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex -= 1;

		if (m_characterSelectionIndex < 0)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShip->applyAnimation(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(leftArrow);
	auto rightArrow = std::make_shared<Text>(">", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(54.0f, 60.0f));
	rightArrow->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_characterSelectionIndex += 1;

		if (m_characterSelectionIndex > static_cast<int>(m_playerModels.size()) - 1)
		{
			m_characterSelectionIndex = static_cast<int>(m_playerModels.size()) - 1;
		}

		selectedShip->applyAnimation(m_playerModels[m_characterSelectionIndex]);
	};
	m_texts.push_back(rightArrow);
	
	auto backOption = std::make_shared<Text>("Back", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(20.0f, 20.0f));
	backOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	};
	m_texts.push_back(backOption);
	auto startOption = std::make_shared<Text>("Start", glm::vec4(255.0f, 160.0f, 122.0f, 1.0f), glm::vec2(30.0f, 20.0f));
	startOption->onMouseReleaseFunc = [=]()
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/1.wav", GL_FALSE);

		m_sceneStateMachine->remove(ScenesEnum::GAME_LIVE);
		m_sceneStateMachine->add(ScenesEnum::GAME_LIVE, std::make_shared<GameScene>(m_sceneStateMachine, m_spriteSheetManager, m_inputManager, m_soundEngine));

		m_sceneStateMachine->switchTo(ScenesEnum::GAME_LIVE);
	};
	m_texts.push_back(startOption);
}

void PickYourCharacterMenu::onActivate()
{
	m_inputManager->clearEverything();
}

void PickYourCharacterMenu::onDestroy() { }

void PickYourCharacterMenu::processInput()
{
	if (m_inputManager->getKey(27))
	{
		m_soundEngine->play2D("assets/Sounds/buttonselect/5.wav", GL_FALSE);

		m_sceneStateMachine->switchTo(ScenesEnum::MAIN);
	}
}

void PickYourCharacterMenu::draw(const std::shared_ptr<Renderer>& renderer, const float dt)
{
	for (auto& text : m_texts)
	{
		text->update(m_inputManager);
	}

	for (auto& uiElement : m_elements)
	{
		uiElement->update(dt, m_inputManager);
	}

    renderer->draw(m_texts);

    for (auto& uiElement : m_elements)
    {
        renderer->draw(uiElement);
    }
}