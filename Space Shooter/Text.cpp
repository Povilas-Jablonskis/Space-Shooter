#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(std::string _text, glm::vec4 _color, glm::vec2 _positionPerc, std::shared_ptr<UIInputComponent> input) :
		UIElementBase(_color, _positionPerc, input), leftButtonClicked(0), text(_text), needUpdate(true), keyBindingInputComponent(nullptr)
	{
		
	}

	Text::Text(std::string _text, glm::vec4 _color, glm::vec2 _positionPerc, std::shared_ptr<UIInputComponent> input, std::shared_ptr<KeyBindingInputComponent> kbInput) :
		UIElementBase(_color, _positionPerc, input), leftButtonClicked(0), text(_text), needUpdate(true), keyBindingInputComponent(kbInput)
	{

	}

	void Text::onHoverEnterFuncDefaults()
	{
		changeColor(glm::vec4(0.0f, 0.0f, 0.0f, getColor().a));
	}

	void Text::onHoverExitFuncDefaults()
	{
		changeColor(glm::vec4(255.0f, 160.0f, 122.0f, getColor().a));
	}

	bool Text::checkIfCollides(glm::vec2 colCoordinates)
	{
		if (colCoordinates.x >= bbox.x && colCoordinates.x <= bbox.y && colCoordinates.y <= bbox.z && colCoordinates.y >= bbox.a)
			return true;
		return false;
	}

	void Text::update(float dt, std::shared_ptr<ConfigurationManager> configurationManager, std::shared_ptr<InputManager> inputManager)
	{
		if (!needUpdate) return;

		cachedCharacters.clear();
		needUpdate = false;

		fixPosition();
		getUIInputComponent()->update(this, inputManager);

		if (keyBindingInputComponent != nullptr)
		{
			keyBindingInputComponent->update(this, inputManager);
		}

		bbox.x = getPosition().x;
		bbox.a = getPosition().y;

		auto lastPosition = getPosition();
		std::vector<int> tempVector;

		for (auto c = text.begin(); c != text.end(); c++)
		{
			auto ch = configurationManager->getInterfaceFont()->getCharacter(*c);

			GLfloat xpos = getPosition().x + ch.Bearing.x;
			GLfloat ypos = getPosition().y - (ch.Size.y - ch.Bearing.y);

			tempVector.push_back(ch.Size.y);

			GLfloat w = (GLfloat)ch.Size.x;
			GLfloat h = (GLfloat)ch.Size.y;
			// Update VBO for each character

			std::vector<cachedCharacter> textVector;
			std::vector<GLfloat> vertices;
			vertices.push_back(xpos);
			vertices.push_back(ypos + h);
			vertices.push_back(0.0);
			vertices.push_back(0.0);

			vertices.push_back(xpos);
			vertices.push_back(ypos);
			vertices.push_back(0.0);
			vertices.push_back(1.0);

			vertices.push_back(xpos + w);
			vertices.push_back(ypos);
			vertices.push_back(1.0);
			vertices.push_back(1.0);


			vertices.push_back(xpos);
			vertices.push_back(ypos + h);
			vertices.push_back(0.0);
			vertices.push_back(0.0);

			vertices.push_back(xpos + w);
			vertices.push_back(ypos);
			vertices.push_back(1.0);
			vertices.push_back(1.0);

			vertices.push_back(xpos + w);
			vertices.push_back(ypos + h);
			vertices.push_back(1.0);
			vertices.push_back(0.0);

			cachedCharacters.push_back(cachedCharacter
			(
				ch.TextureID,
				vertices
			));
			setPosition(0, getPosition().x + (ch.Advance >> 6)); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		bbox.y = getPosition().x;
		bbox.z = getPosition().y + (tempVector.size() == 0 ? 0.0f : (float)*std::max_element(std::begin(tempVector), std::end(tempVector)));
		setPosition(lastPosition);
	}

	void Text::fixPosition()
	{
		float windowWidth = (float)glutGet(GLUT_WINDOW_WIDTH);
		float windowHeight = (float)glutGet(GLUT_WINDOW_HEIGHT);

		if (getPositionPercents() != glm::vec2(0.0f, 0.0f))
		{
			setPosition(0, windowWidth * (getPositionPercents().x / 100.0f));
			setPosition(1, windowHeight * (getPositionPercents().y / 100.0f));
		}
	}
}