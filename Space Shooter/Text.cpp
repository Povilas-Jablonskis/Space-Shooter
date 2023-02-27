#include "Text.hpp"
#include "Font.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <freeglut/freeglut.h>

Text::Text(std::string text, const glm::vec4& color, const glm::vec2& positionPercents, Font& font) :
	m_font(font), m_text(std::move(text)), m_positionPercents(positionPercents), m_color(color)
{
	onHoverEnterFunc = []
	{
	};

	onHoverExitFunc = []
	{
	};

	onMouseClickFunc = []
	{
	};

	onMouseReleaseFunc = []
	{
	};
}

void Text::update(const InputManager& inputManager)
{
	if (!doesItNeedUpdate()) return;

	m_cachedCharacters.clear();
	setNeedUpdate(false);

	fixPosition();

	m_bbox[0] = getPosition().x;
	m_bbox[3] = getPosition().y;

	const auto& lastPosition = getPosition();
	std::vector<float> tempVector;

	for (const signed char c : m_text)
	{
		auto ch = m_font.getCharacter(c);

		auto xpos = getPosition().x + ch.Bearing.x;
		auto ypos = getPosition().y - (ch.Size.y - ch.Bearing.y);

		tempVector.push_back(ch.Size.y);

		const auto w = ch.Size.x;
		const auto h = ch.Size.y;
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
		setPosition(0, getPosition().x + static_cast<float>(ch.Advance >> 6));
		// Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	m_bbox[1] = getPosition().x;
	m_bbox[2] = getPosition().y + (tempVector.empty()
		                               ? 0.0f
		                               : *std::ranges::max_element(tempVector));
	setPosition(lastPosition);

	if (!isActive())
	{
		return;
	}

	if (inputManager.getLastMousePosition().x <=
		getBoundingBox().y && inputManager.getLastMousePosition().y <= getBoundingBox().z && inputManager.
		getLastMousePosition().y >= getBoundingBox().a)
	{
		if (!isClickedByMouse())
		{
			if (!inputManager.getLastLeftMouseState() && inputManager.getLeftMouseState())
			{
				onMouseClickFunc();
				setMousedClicked(true);
			}
		}
		else
		{
			if (inputManager.getLastLeftMouseState() && !inputManager.getLeftMouseState())
			{
				onMouseReleaseFunc();
				setMousedClicked(false);
			}
		}

		if (!isHoveredByMouse())
		{
			onHoverEnterFuncDefaults();
			onHoverEnterFunc();
			setMousedHovered(true);
		}
	}
	else
	{
		if (isHoveredByMouse())
		{
			onHoverExitFuncDefaults();
			onHoverExitFunc();
			setMousedHovered(false);
		}
	}
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

void Text::onHoverEnterFuncDefaults()
{
	changeColor(glm::vec4(0.0f, 0.0f, 0.0f, getColor().a));
}

void Text::onHoverExitFuncDefaults()
{
	changeColor(glm::vec4(255.0f, 160.0f, 122.0f, getColor().a));
}

const std::vector<cachedCharacter>& Text::getCachedCharacters() const
{
	return m_cachedCharacters;
}

const std::string& Text::getText() const
{
	return m_text;
}

const glm::vec4& Text::getBoundingBox() const
{
	return m_bbox;
}

bool Text::doesItNeedUpdate() const
{
	return m_needUpdate;
}

void Text::setNeedUpdate(const bool t_needUpdate)
{
	m_needUpdate = t_needUpdate;
}

void Text::setText(const std::string& newtext)
{
	m_needUpdate = true;
	m_text = newtext;
}

void Text::setPosition(const glm::vec2& position)
{
	m_position = position;
	m_needUpdate = true;
}

void Text::setPosition(const int index, const float position)
{
	m_position[index] = position;
	m_needUpdate = true;
}

void Text::disable()
{
	m_active = false;
}

void Text::enable()
{
	m_active = true;
}

bool Text::isActive() const
{
	return m_active;
}

void Text::setMousedHovered(const bool boolean)
{
	m_gotMousedHovered = boolean;
}

bool Text::isHoveredByMouse() const
{
	return m_gotMousedHovered;
}

void Text::setMousedClicked(const bool boolean)
{
	m_gotMousedClicked = boolean;
}

bool Text::isClickedByMouse() const
{
	return m_gotMousedClicked;
}

const glm::vec2& Text::getPositionPercents() const
{
	return m_positionPercents;
}

void Text::changeColor(const float color, const int index)
{
	m_color[index] = color;
}

void Text::changeColor(const glm::vec4& color)
{
	m_color = color;
}

const glm::vec4& Text::getColor() const
{
	return m_color;
}

const glm::vec2& Text::getPosition() const
{
	return m_position;
}

float Text::getRotationAngle() const
{
	return m_rotationAngle;
}

void Text::setScale(const float scale)
{
	m_scale = scale;
}

void Text::setRotationAngle(const float rotation)
{
	m_rotationAngle = rotation;
}

float Text::getWidth() const
{
	return m_width * m_scale;
}

float Text::getHeight() const
{
	return m_height * m_scale;
}

void Text::setWidth(const float width)
{
	m_width = width;
}

void Text::setHeight(const float height)
{
	m_height = height;
}

float Text::getOriginalWidth() const
{
	return m_originalWidth;
}

float Text::getOriginalHeight() const
{
	return m_originalHeight;
}

void Text::setOriginalWidth(const float width)
{
	m_originalWidth = width;
}

void Text::setOriginalHeight(const float height)
{
	m_originalHeight = height;
}
