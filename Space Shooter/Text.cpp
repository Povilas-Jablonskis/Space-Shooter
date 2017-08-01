#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, std::shared_ptr<Font> _font, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), leftButtonClicked(0), fontSize(_fontsize), text(_text), font(_font), lastText("")
	{
		cache = font->getCharacterList();
	}

	Text::~Text()
	{

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
}