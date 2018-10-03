#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(std::string _text, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<Font> _font, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), leftButtonClicked(0), text(_text), font(_font), needUpdate(true)
	{
	}

	Text::Text(const char _text, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<Font> _font, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), leftButtonClicked(0), text(""), font(_font), needUpdate(true)
	{
		text += _text;
	}

	Text::~Text()
	{
		cachedCharacters.clear();
	}

	void Text::onHoverEnterFuncDefaults()
	{
		color.r = 0.0f;
		color.g = 0.0f;
		color.b = 0.0f;
	}

	void Text::onHoverExitFuncDefaults()
	{
		color.r = 255.0f;
		color.g = 160.0f;
		color.b = 122.0f;
	}

	bool Text::checkIfCollides(glm::vec2 colCoordinates)
	{
		if (colCoordinates.x >= bbox.x && colCoordinates.x <= bbox.y && colCoordinates.y <= bbox.z && colCoordinates.y >= bbox.a)
			return true;
		return false;
	}

	void Text::update(float dt)
	{
		if (color.a == 0.0f || font == nullptr || !needUpdate) return;

		cachedCharacters.clear();
		needUpdate = false;

		bbox.x = position.x;
		bbox.a = position.y;

		auto lastPosition = position;
		std::vector<int> tempVector;

		for (auto c = text.begin(); c != text.end(); c++)
		{
			auto ch = font->getCharacter(*c);

			GLfloat xpos = position.x + ch.Bearing.x;
			GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y);

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
			position.x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		bbox.y = position.x;
		bbox.z = position.y + (tempVector.size() == 0 ? 0.0f : (float)*std::max_element(std::begin(tempVector), std::end(tempVector)));
		position = lastPosition;
	}

	void Text::fixPosition()
	{
		glm::vec2 temPos = glm::vec2((float)(glutGet(GLUT_WINDOW_WIDTH)), (float)(glutGet(GLUT_WINDOW_HEIGHT)));

		if (positionPercents == glm::vec2(0.0f, 0.0f))
			return;

		setPosition(0, temPos.x * (positionPercents.x / 100.0f));
		setPosition(1, temPos.y * (positionPercents.y / 100.0f));
	}

	void Text::draw(std::shared_ptr<Renderer> renderer)
	{
		if (getColor(3) == 0.0f || getFont() == nullptr) return;
		auto program = renderer->getShaderProgram("textshader");
		auto offsetLocation = glGetUniformLocation(program, "color");
		auto offsetLocation2 = glGetUniformLocation(program, "projection");
		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);
		glBindVertexArray(renderer->getTextVAO());
		glUseProgram(program);
		if (getColor(3) == 0.0f || getFont() == nullptr) return;

		glUniform4f(offsetLocation, getColor(0) / 255.0f, getColor(1) / 255.0f, getColor(2) / 255.0f, getColor(3));
		glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

		auto cache = getCachedCharacters();

		for (auto it = cache.begin(); it != cache.end(); it++)
		{
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, it->first);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, renderer->getTextVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, &it->second[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glUseProgram(0);
		glBindVertexArray(0);
	}
}