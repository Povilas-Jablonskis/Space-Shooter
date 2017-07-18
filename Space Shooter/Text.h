#ifndef _textH
#define _textH

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <functional>

#include "UIElementBase.h"
#include "Application.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			Text(const std::string&, int, glm::vec2, glm::vec4, FT_Face*, bool, glm::vec2);
			~Text();
			void draw();
			void update(InputManager*);
			void onHoverEnterFuncDefaults();
			void onHoverExitFuncDefaults();
			void onMouseClickDefaults(InputManager*);
			void onMouseReleaseFuncDefaults(InputManager*);
		private:
			std::string text;
			int fontSize;
			FT_Face* face;
			bool mouseOnText;
			SHORT leftButtonClicked;
			float bbox[4];
			bool isStatic;
		};
}
#endif