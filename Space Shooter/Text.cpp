#include "Text.hpp"

#include <string>
#include <algorithm>
#include <iostream>
#include <freeglut/freeglut.h>

#include "Colors.hpp"

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

void Text::update()
{
	if (! doesNeedUpdate()) return;

	m_cachedCharacters.clear();
	setNeedUpdate(false);

	updatePosition();

	m_bbox[0] = getPosition().x;
	m_bbox[3] = getPosition().y;

	const auto& lastPosition = getPosition();
	std::vector<float> tempVector;

	for (const signed char c : m_text)
	{
		auto [textureID, size, bearing, advance] = m_font.getCharacter(c);

		auto positionX = getPosition().x + bearing.x;
		auto positionY = getPosition().y - (size.y - bearing.y);

		tempVector.push_back(size.y);

		const auto w = size.x;
		const auto h = size.y;

		std::vector<GLfloat> vertices;
		vertices.push_back(positionX);
		vertices.push_back(positionY + h);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(positionX);
		vertices.push_back(positionY);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(positionX + w);
		vertices.push_back(positionY);
		vertices.push_back(1.0);
		vertices.push_back(1.0);


		vertices.push_back(positionX);
		vertices.push_back(positionY + h);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(positionX + w);
		vertices.push_back(positionY);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

		vertices.push_back(positionX + w);
		vertices.push_back(positionY + h);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		m_cachedCharacters.emplace_back(
			textureID,
			vertices
		);
		setPosition(0, getPosition().x + static_cast<float>(advance >> 6));
		// Bit shift by 6 to get value in pixels (2^6 = 64)
	}
	m_bbox[1] = getPosition().x;
	m_bbox[2] = getPosition().y + (tempVector.empty()
		                               ? 0.0f
		                               : *std::ranges::max_element(tempVector));
	setPosition(lastPosition);
}

void Text::updatePosition()
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
	changeColor(Colors::BLACK);
}

void Text::onHoverExitFuncDefaults()
{
	changeColor(Colors::DEFAULT_TEXT);
}

const std::vector<CachedCharacter>& Text::getCachedCharacters() const
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

bool Text::doesNeedUpdate() const
{
	return m_needUpdate;
}

void Text::setNeedUpdate(const bool needUpdate)
{
	m_needUpdate = needUpdate;
}

void Text::setText(const std::string& text)
{
	m_needUpdate = true;
	m_text = text;
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

void Text::setMousedHovered(const bool gotMousedHovered)
{
	m_gotMousedHovered = gotMousedHovered;
}

bool Text::isHoveredByMouse() const
{
	return m_gotMousedHovered;
}

void Text::setMousedClicked(const bool gotMousedClicked)
{
	m_gotMousedClicked = gotMousedClicked;
}

bool Text::isClickedByMouse() const
{
	return m_gotMousedClicked;
}

const glm::vec2& Text::getPositionPercents() const
{
	return m_positionPercents;
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
