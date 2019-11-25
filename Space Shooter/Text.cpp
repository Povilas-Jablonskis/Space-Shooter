#include "Text.hpp"
#include "ConfigurationManager.hpp"
#include "UIInputComponent.hpp"
#include "Font.hpp"
#include "KeyBindingInputComponent.hpp"

namespace Engine
{
	Text::Text(const std::string& text, const glm::vec4& color, const glm::vec2& positionPerc, const std::shared_ptr<KeyBindingInputComponent>& kbInput) : UIElementBase(color, positionPerc), m_text(text), m_keyBindingInputComponent(kbInput)
	{

	}

	Text::Text(const std::string& text, const glm::vec4& color, const glm::vec2& positionPerc) : UIElementBase(color, positionPerc), m_text(text), m_keyBindingInputComponent(nullptr)
	{
		
	}

	void Text::setPosition(const glm::vec2& position)
	{ 
		RenderObject::setPosition(position);
		m_needUpdate = true; 
	}

	void Text::setPosition(int index, float position)
	{ 
		RenderObject::setPosition(index, position);
		m_needUpdate = true;
	}

	void Text::onHoverEnterFuncDefaults()
	{
		changeColor(glm::vec4(0.0f, 0.0f, 0.0f, getColor().a));
	}

	void Text::onHoverExitFuncDefaults()
	{
		changeColor(glm::vec4(255.0f, 160.0f, 122.0f, getColor().a));
	}

	bool Text::checkIfCollides(const glm::vec2& colCoordinates)
	{
		return colCoordinates.x >= getBoundaryBox().x && colCoordinates.x <= getBoundaryBox().y && colCoordinates.y <= getBoundaryBox().z && colCoordinates.y >= getBoundaryBox().a;
	}

	void Text::update(float dt, const std::unique_ptr<ConfigurationManager>& configurationManager, const std::unique_ptr<InputManager>& inputManager)
	{
		if (!doesItNeedUpdate()) return;

		m_cachedCharacters.clear();
		setNeedUpdate(false);

		fixPosition();
		getUIInputComponent()->update(this, inputManager);

		if (getKeyBindingInputComponent() != nullptr)
		{
			getKeyBindingInputComponent()->update(this, inputManager);
		}

		changeBoundaryBox(getPosition().x, 0);
		changeBoundaryBox(getPosition().y, 3);

		auto lastPosition = getPosition();
		std::vector<int> tempVector;
		auto text = getText();

		for (auto c = text.begin(); c != text.end(); ++c)
		{
			auto ch = configurationManager->getInterfaceFont()->getCharacter(*c);

			GLfloat xpos = getPosition().x + ch.Bearing.x;
			GLfloat ypos = getPosition().y - (ch.Size.y - ch.Bearing.y);

			tempVector.push_back(ch.Size.y);

			GLfloat w = static_cast<GLfloat>(ch.Size.x);
			GLfloat h = static_cast<GLfloat>(ch.Size.y);
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

			m_cachedCharacters.push_back(cachedCharacter
			(
				ch.TextureID,
				vertices
			));
			setPosition(0, getPosition().x + (ch.Advance >> 6)); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		changeBoundaryBox(getPosition().x, 1);
		changeBoundaryBox(getPosition().y + (tempVector.empty() ? 0.0f : static_cast<float>(*std::max_element(std::begin(tempVector), std::end(tempVector)))), 2);
		setPosition(lastPosition);
	}

	void Text::fixPosition()
	{
		float windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

		if (getPositionPercents() != glm::vec2(0.0f, 0.0f))
		{
			setPosition(0, windowWidth * (getPositionPercents().x / 100.0f));
			setPosition(1, windowHeight * (getPositionPercents().y / 100.0f));
		}
	}
}