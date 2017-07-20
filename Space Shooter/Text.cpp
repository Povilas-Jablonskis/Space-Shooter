#include "Text.h"
#include <cctype>
#include <algorithm>

namespace Engine
{
	Text::Text(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, FT_FaceRec_* _face, bool _isStatic, glm::vec2 _positionPerc) :
		UIElementBase(0, 0, _position, _color, _positionPerc), mouseOnText(false), leftButtonClicked(0), fontSize(_fontsize), text(_text), face(_face), isStatic(_isStatic)
	{

	}

	Text::~Text()
	{

	}

	void Text::draw(GLuint program, GLuint textTexture, GLuint vbo)
	{
		if (color.a == 0.0f) return;
			
		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		float sx = 2.0f / windowWidth;
		float sy = 2.0f / windowHeigth;
		float x = -1 + position.x * sx;
		float y = -1 + position.y * sy;

		bbox[0] = position.x;
		bbox[1] = position.x;
		bbox[2] = position.y;
		bbox[3] = position.y;

		const char *p;
		FT_FaceRec_ tempFace = *face;
		FT_GlyphSlot g = tempFace.glyph;

		/* Set font size */
		FT_Set_Pixel_Sizes(&tempFace, 0, fontSize);

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
			if (FT_Load_Char(&tempFace, *p, FT_LOAD_RENDER))
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
	}

	void Text::update(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
		{
			if (!mouseOnText)
			{
				onHoverEnterFunc();
				onHoverEnterFuncDefaults();
				mouseOnText = true;
			}
		}
		else
		{
			if (mouseOnText)
			{
				onHoverExitFunc();
				onHoverExitFuncDefaults();
				mouseOnText = false;
			}
		}
	}

	void Text::onHoverEnterFuncDefaults()
	{
		color.r = 0.0f;
		color.g = 0.0f;
		color.b = 0.0f;
	}

	void Text::onHoverExitFuncDefaults()
	{
		color.r = 255.0f;
		color.g = 160.0f;
		color.b = 122.0f;
	}

	void Text::onMouseClickDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			onMouseClickFunc();
	}

	void Text::onMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f || isStatic) return;

		glm::vec2 lastMousePosition = inputManager->getLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= bbox[0] && lastMousePosition.x <= bbox[1] && lastMousePosition.y <= bbox[2] && lastMousePosition.y >= bbox[3])
			onMouseReleaseFunc();
	}
}