#include "Text.hpp"
#include "ConfigurationManager.hpp"
#include "UIInputComponent.hpp"
#include "Font.hpp"
#include "KeyBindingInputComponent.hpp"

#include <algorithm>
#include <utility>

namespace Engine
{
	Text::Text(std::string text, const glm::vec4& color, const glm::vec2& positionPerc,
	           std::shared_ptr<KeyBindingInputComponent> kbInput) : UIElementBase(color, positionPerc), m_text(
		                                                                std
		                                                                ::
		                                                                move(
			                                                                text)), m_keyBindingInputComponent(
		                                                                std::move(kbInput))
	{

	}

	Text::Text(std::string text, const glm::vec4& color, const glm::vec2& positionPerc) : UIElementBase(color, positionPerc), m_text(
		                                                                                      std::move(text)), m_keyBindingInputComponent(nullptr)
	{
		
	}

	void Text::setPosition(const glm::vec2& position)
	{ 
		RenderObject::setPosition(position);
		m_needUpdate = true; 
	}

	void Text::setPosition(const int index, const float position)
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

	bool Text::checkIfCollides(const glm::vec2& colCoordinates) const
	{
		return colCoordinates.x >= getBoundaryBox().x && colCoordinates.x <= getBoundaryBox().y && colCoordinates.y <= getBoundaryBox().z && colCoordinates.y >= getBoundaryBox().a;
	}

	void Text::update(const std::shared_ptr<ConfigurationManager>& configurationManager, const std::shared_ptr<InputManager>& inputManager)
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

		const auto lastPosition = getPosition();
		std::vector<int> tempVector;
		auto text = getText();

		for (std::_String_iterator<std::_String_val<std::_Simple_types<char>>>::value_type& c : text)
		{
			auto ch = configurationManager->getInterfaceFont()->getCharacter(c);

			auto xpos = getPosition().x + ch.Bearing.x;
			auto ypos = getPosition().y - (ch.Size.y - ch.Bearing.y);

			tempVector.push_back(ch.Size.y);

			const auto w = static_cast<GLfloat>(ch.Size.x);
			const auto h = static_cast<GLfloat>(ch.Size.y);
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

			m_cachedCharacters.emplace_back(
				ch.TextureID,
				vertices
			);
			setPosition(0, getPosition().x + (ch.Advance >> 6)); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		changeBoundaryBox(getPosition().x, 1);
		changeBoundaryBox(getPosition().y + (tempVector.empty() ? 0.0f : static_cast<float>(*std::max_element(std::begin(tempVector), std::end(tempVector)))), 2);
		setPosition(lastPosition);
	}

	void Text::fixPosition()
	{
		const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
		const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

		if (getPositionPercents() != glm::vec2(0.0f, 0.0f))
		{
			setPosition(0, windowWidth * (getPositionPercents().x / 100.0f));
			setPosition(1, windowHeight * (getPositionPercents().y / 100.0f));
		}
	}
}