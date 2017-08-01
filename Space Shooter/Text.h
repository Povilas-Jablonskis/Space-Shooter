#ifndef _textH
#define _textH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include "mat4x4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include FT_FREETYPE_H
#include <functional>

#include "UIElementBase.h"
#include "InputManager.h"
#include "Font.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			Text(const std::string&, int, glm::vec2, glm::vec4, std::shared_ptr<Font>, glm::vec2);
			~Text();
			inline std::shared_ptr<Font> getFont() const { return font; }
			inline std::string getText() const { return text; }
			inline void setText(std::string newtext) { text = newtext; }
			inline void setLastText(std::string newtext) { lastText = newtext; }
			inline std::string getLastText() const { return lastText; }
			inline std::map<GLchar, Character> getCache() const { return cache; }
			inline void setBBOXVar(int index, float value) { bbox[index] = value; }
			bool checkIfCollides(glm::vec2);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
		private:
			std::string lastText;
			std::string text;
			int fontSize;
			std::map<GLchar, Character> cache;
			std::shared_ptr<Font> font;
			SHORT leftButtonClicked;
			float bbox[4];
	};
}
#endif