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
			void draw(GLuint, GLuint);
			bool checkIfCollides(glm::vec2);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
		private:
			std::string lastText;
			std::string text;
			int fontSize;
			std::map<GLchar, Character> cache;
			std::map<GLchar, Character> cachedCharacters;
			std::shared_ptr<Font> font;
			SHORT leftButtonClicked;
			float bbox[4];
	};
}
#endif