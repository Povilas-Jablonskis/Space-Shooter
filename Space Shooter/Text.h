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
			Text(const std::string&, int, glm::vec2, glm::vec4, const std::string&, bool, glm::vec2, std::shared_ptr<Application>);
			~Text();
			void draw();
			void update();
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults();
			void onMouseReleaseFuncDefaults();
		private:
			std::string text;
			int fontSize;
			std::string font;
			bool mouseOnText;
			SHORT leftButtonClicked;
			float bbox[4];
			bool isStatic;
	};
}
#endif