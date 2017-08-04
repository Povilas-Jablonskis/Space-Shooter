#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<Font> _font, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), leftButtonClicked(0), fontSize(_fontsize), text(_text), font(_font), lastText(""), needUpdate(true)
	{
		
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
		if (colCoordinates.x >= bbox[0] && colCoordinates.x <= bbox[1] && colCoordinates.y <= bbox[2] && colCoordinates.y >= bbox[3])
			return true;
		return false;
	}

	void Text::update(float dt)
	{
		if (color.a == 0.0f || font == nullptr || !needUpdate) return;

		cachedCharacters.clear();
		needUpdate = false;

		bbox[0] = position.x;
		bbox[3] = position.y;

		auto lastPosition = position;
		std::vector<int> tempVector;
		auto cache = font->getCharacterList();

		for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
		{
			Character ch = cache.at(*c);

			GLfloat xpos = position.x + ch.Bearing.x;
			GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y);

			tempVector.push_back(ch.Size.y);

			GLfloat w = (GLfloat)ch.Size.x;
			GLfloat h = (GLfloat)ch.Size.y;
			// Update VBO for each character
			struct StructForTextCache tempStruct = 
			{ 
				ch.TextureID,
				{
					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos, ypos, 0.0, 1.0 },
					{ xpos + w, ypos, 1.0, 1.0 },

					{ xpos, ypos + h, 0.0, 0.0 },
					{ xpos + w, ypos, 1.0, 1.0 },
					{ xpos + w, ypos + h, 1.0, 0.0 }
				} 
			};

			cachedCharacters.push_back(tempStruct);
			position.x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		bbox[1] = position.x;
		bbox[2] = position.y + (float)*std::max_element(std::begin(tempVector), std::end(tempVector));
		position = lastPosition;
	}
}