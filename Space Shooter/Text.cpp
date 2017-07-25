#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<Font> _font, bool _isStatic, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), mouseOnText(false), leftButtonClicked(0), fontSize(_fontsize), text(_text), font(_font), isStatic(_isStatic)
	{

	}

	Text::~Text()
	{

	}

	void Text::draw(GLuint program, GLuint vbo)
	{
		if (color.a == 0.0f || font == nullptr) return;

		bbox[0] = position.x;
		bbox[1] = position.x;
		bbox[2] = position.y;
		bbox[3] = position.y;

		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "projection");
		glUniform4f(offsetLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);
		glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

		auto lastPosition = position;

		// Iterate through all characters
		std::string::const_iterator c;
		const std::map<GLchar, Character>* tempCharacterList = font->GetCharacterList();
		std::vector<int> tempVector;
		for (auto character : *tempCharacterList)
			tempVector.push_back(character.second.Size.y);
		bbox[2] = position.y +(float)*std::max_element(std::begin(tempVector), std::end(tempVector));
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = tempCharacterList->at(*c);

			GLfloat xpos = position.x + ch.Bearing.x;
			GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y);

			GLfloat w = (GLfloat)ch.Size.x;
			GLfloat h = (GLfloat)ch.Size.y;
			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			position.x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)

		}

		bbox[1] = position.x;
		position = lastPosition;
	}

	void Text::update(std::shared_ptr<InputManager> inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
		{
			if (!mouseOnText)
			{
				onHoverEnterFunc();
				onHoverEnterFuncDefaults();
				mouseOnText = true;
			}
		}
		else
		{
			if (mouseOnText)
			{
				onHoverExitFunc();
				onHoverExitFuncDefaults();
				mouseOnText = false;
			}
		}
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

	void Text::onMouseClickDefaults(std::shared_ptr<InputManager> inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			onMouseClickFunc();
	}

	void Text::onMouseReleaseFuncDefaults(std::shared_ptr<InputManager> inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			onMouseReleaseFunc();
	}
}