#include "Text.hpp"
#include "RenderObject.hpp"
#include "Font.hpp"
#include "checkCollision.hpp"

#include <string>
#include <algorithm>
#include <utility>
#include <freeglut/freeglut_std.h>

Text::Text(std::string text, const glm::vec4& color, const glm::vec2& positionPerc) : UIElementBase(color, positionPerc), m_text(
	std::move(text))
{
	FT_Library library;

	if (FT_Init_FreeType(&library))
	{
#if _DEBUG
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
#endif
	}

	FT_Face face;
	if (FT_New_Face(library, "assets/Fonts/kenvector_future_thin.ttf", 0, &face))
	{
#if _DEBUG
		std::cout << "ERROR::FREETYPE: Failed to load font\n";
#endif
	}

	m_font = std::make_shared<Font>(face);

	// We Don't Need The Face Information Now That The Display
	// Lists Have Been Created, So We Free The Associated Resources.
	FT_Done_Face(face);

	// Ditto For The Font Library.
	FT_Done_FreeType(library);
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

void Text::update(const std::shared_ptr<InputManager>& inputManager)
{
	if (!doesItNeedUpdate()) return;

	m_cachedCharacters.clear();
	setNeedUpdate(false);

	fixPosition();

	m_bbox[0] = getPosition().x;
	m_bbox[3] = getPosition().y;

	const auto& lastPosition = getPosition();
	std::vector<float> tempVector;
	auto& text = getText();

	for (FT_ULong c : m_text)
	{
		auto ch = m_font->getCharacter(c);

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
	m_bbox[1] = getPosition().x;
	m_bbox[2] = getPosition().y + (tempVector.empty() ? 0.0f : static_cast<float>(*std::max_element(std::begin(tempVector), std::end(tempVector))));
	setPosition(lastPosition);

	checkCollision(this, inputManager);
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