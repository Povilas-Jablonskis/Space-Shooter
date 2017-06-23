#ifndef _Text
#define _Text

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

#include "UIElementBase.h"
#include "FontLoader.h"

namespace Engine
{
	class Text : public UIElementBase
	{
		public:
			~Text();
			Text(std::string, int, float, float, float, float, float, float, std::string);
			Text();
			void Draw(GLuint, UIElementBase*);
		private:
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
		};
}
#endif