#ifndef _Text
#define _Text

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
#include <functional>

#include "UIElementBase.h"
#include "FontLoader.h"

#include <vec2.hpp>
#include <vec4.hpp>

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			~Text();
			Text(const std::string&, int, glm::vec2, glm::vec4, const std::string&);
			Text();
			void Draw(GLuint, UIElementBase*);
			void Update();
			std::function<void()> OnHoverEnterFunc;
			std::function<void()> OnHoverExitFunc;
			std::function<void()> OnMouseClickFunc;
			std::function<void()> OnMouseReleaseFunc;
		private:
			void OnHoverEnterFuncDefaults();
			void OnHoverExitFuncDefaults();
			void OnMouseClickDefaults();
			void OnMouseReleaseFuncDefaults();
			struct point
			{
				GLfloat x;
				GLfloat y;
				GLfloat s;
				GLfloat t;
			};
			std::string fontname;
			std::string text;
			int textsize;
			GLuint texture;
			GLuint vbo;
			bool mouseontext;
			SHORT leftbuttonclicked;
			float bbox[4];
		};
}
#endif