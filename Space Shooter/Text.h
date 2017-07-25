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
			Text(const std::string&, int, glm::vec2, glm::vec4, std::shared_ptr<Font>, bool, glm::vec2);
			~Text();
			void draw(GLuint, GLuint);
			void update(std::shared_ptr<InputManager>);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults(std::shared_ptr<InputManager>);
			void onMouseReleaseFuncDefaults(std::shared_ptr<InputManager>);
		private:
			std::string text;
			int fontSize;
			std::shared_ptr<Font> font;
			bool mouseOnText;
			SHORT leftButtonClicked;
			float bbox[4];
			bool isStatic;
		};
}
#endif