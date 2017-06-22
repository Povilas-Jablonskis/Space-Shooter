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
			Text(const char*, int, float, float, float, float, float, const char* font, UIElementBase*);
			Text();
			void Draw(GLuint);
		private:
			struct point
			{
				GLfloat x;
				GLfloat y;
				GLfloat s;
				GLfloat t;
			};
			const char* fontpath;
			const char* text;
			int textsize;
			GLuint texture;
			GLuint vbo;
			UIElementBase* parent;
		};
}
#endif