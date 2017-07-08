#include "Text.h"
#include <cctype>
#include <algorithm>
#include "Application.h"

namespace Engine
{
	Text::Text()
	{
		
	}
		
	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, FT_Face* _face) :
		UIElementBase(0, 0, _position, _color), mouseontext(false), leftbuttonclicked(0), fontsize(_fontsize), text(_text), face(_face), isStatic(false)
	{

	}

	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, FT_Face* _face, bool _isStatic) :
		UIElementBase(0, 0, _position, _color), mouseontext(false), leftbuttonclicked(0), fontsize(_fontsize), text(_text), face(_face), isStatic(_isStatic)
	{

	}

	Text::~Text()
	{

	}

	void Text::Draw()
	{
		auto program = Application::GetShaderProgram("textshader");
		glUseProgram(program);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, Application::GetTextTexture());

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			/* We require 1 byte alignment when uploading texture data */
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			/* Clamping to edges is important to prevent artifacts when scaling */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			/* Linear filtering usually looks best for text */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindBuffer(GL_ARRAY_BUFFER, Application::GetTextVBO());
				float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				float sx = 2.0f / windowwidth;
				float sy = 2.0f / windowheigth;
				/*float x = -1 + (windowwidth * position.x) * sx;
				float y = -1 + (windowheigth * position.y) * sy;*/
				float x = -1 + position.x * sx;
				float y = -1 + position.y * sy;

				bbox[0] = position.x;
				bbox[1] = position.x;
				bbox[2] = position.y;
				bbox[3] = position.y;
				/*if (parent != nullptr)
				{
					float startx = parent->GetPosition(0) * windowwidth;
					float endx = startx + parent->GetSize(0);
					float starty = parent->GetPosition(1) * windowheigth;
					float endy = starty + parent->GetSize(1);

					float xdiff = endx - startx;
					float ydiff = endy - starty;

					bbox[0] = ((startx + (xdiff * position.x)));
					bbox[1] = ((startx + (xdiff * position.x)));
					bbox[2] = ((starty + (ydiff * position.y)));
					bbox[3] = ((starty + (ydiff * position.y)));

					if (position.x == 0.0f)
						x = -1 + startx * sx;
					else
						x = -1 + ((startx + (xdiff * position.x))) * sx;

					if (position.y == 0.0f)
						y = -1 + starty * sy;
					else
						y = -1 + ((starty + (ydiff * position.y))) * sy;
				}*/

				const char *p;
				FT_GlyphSlot g = (*face)->glyph;

				/* Set font size */
				FT_Set_Pixel_Sizes(*face, 0, fontsize);

				bbox[2] += g->face->glyph->metrics.height >> 6;

				/* Create a texture that will be used to hold one "glyph" */
				int offsetLocation2 = glGetUniformLocation(program, "color");
				glUniform4f(offsetLocation2, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

				/* Loop through all characters */
				for (p = text.c_str(); *p; p++)
				{
					/* Try to load and render the character */
					if (FT_Load_Char(*face, *p, FT_LOAD_RENDER))
						continue;

					/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

					bbox[1] += g->metrics.horiAdvance >> 6;

					/* Calculate the vertex and texture coordinates */
					float x2 = x + g->bitmap_left * sx;
					float y2 = -y - g->bitmap_top * sy;
					float w = g->bitmap.width * sx;
					float h = g->bitmap.rows * sy;

					glm::vec4 box[4] = {
						{ x2     , -y2    , 0, 0 },
						{ x2 +  w, -y2    , 1, 0 },
						{ x2     , -y2 - h, 0, 1 },
						{ x2 + w , -y2 - h, 1, 1 },
					};

					glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					/* Advance the cursor to the start of the next character */
					x += (g->advance.x >> 6) * sx;
					y += (g->advance.y >> 6) * sy;
				}
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		glUseProgram(0);
	}

	void Text::Update(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->GetLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
		{
			if (!mouseontext)
			{
				OnHoverEnterFunc();
				OnHoverEnterFuncDefaults();
				mouseontext = true;
			}
		}
		else
		{
			if (mouseontext)
			{
				OnHoverExitFunc();
				OnHoverExitFuncDefaults();
				mouseontext = false;
			}
		}
	}

	void Text::OnHoverEnterFuncDefaults()
	{
		color.r = 0.0f;
		color.g = 0.0f;
		color.b = 0.0f;
	}

	void Text::OnHoverExitFuncDefaults()
	{
		color.r = 255.0f;
		color.g = 160.0f;
		color.b = 122.0f;
	}

	void Text::OnMouseClickDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->GetLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			OnMouseClickFunc();
	}

	void Text::OnMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->GetLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			OnMouseReleaseFunc();
	}
}