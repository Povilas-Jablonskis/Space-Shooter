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
			~Text();
			Text(const std::string&, int, glm::vec2, glm::vec4, FT_Face*);
			Text(const std::string&, int, glm::vec2, glm::vec4, FT_Face*, bool);
			Text();
			void Draw();
			void Update(InputManager*);
			void OnHoverEnterFuncDefaults();
			void OnHoverExitFuncDefaults();
			void OnMouseClickDefaults(InputManager*);
			void OnMouseReleaseFuncDefaults(InputManager*);
		private:
			std::string text;
			int fontsize;
			FT_Face* face;
			bool mouseontext;
			SHORT leftbuttonclicked;
			float bbox[4];
			bool isStatic;
		};
}
#endif