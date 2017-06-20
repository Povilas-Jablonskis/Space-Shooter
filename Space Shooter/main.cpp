#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <map>

#include "BaseGameObject.h"
#include "Shader.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "TestEnemy.h"

#include "document.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include "writer.h"

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

FT_Library library;
FT_Face face;

using namespace Engine;

int prevtime, currtime;
float dt;

GLuint VBO, VAO, EBO;
GLuint vbo;

GLuint texture;

Shader shader;
Shader textshader;
Player player;

std::vector<TestEnemy*> enemies;

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat s;
	GLfloat t;
};


GLfloat vertices[] =
{
	// Positions    // Texture Coords
	1.0f,  1.0f,    1.0f, 1.0f, // Top Right
	1.0f, -1.0f,    1.0f, 0.0f, // Bottom Right
   -1.0f, -1.0f,    0.0f, 0.0f, // Bottom Left
   -1.0f,  1.0f,    0.0f, 1.0f  // Top Left
};

GLuint indices[] =
{   // Note that we start from 0!
	0, 1, 3,  // First Triangle
	1, 2, 3   // Second Triangle
};

void render_text(const char *text, float x, float y, float sx, float sy, int size) {
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Set font size */
	FT_Set_Pixel_Sizes(face, 0, size);

	/* Create a texture that will be used to hold one "glyph" */
	int offsetLocation2 = glGetUniformLocation(textshader.GetShader(), "color");
	glUniform4f(offsetLocation2, 255.0f / 255.0f, 160.0f / 255.0f, 122.0f / 255.0f, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (p = text; *p; p++) 
	{
		/* Try to load and render the character */
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void idle(void)
{
}

void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT);

	//DrawContent();

	glutSwapBuffers();
}

void resize(int width, int height)
{
	//const float ar = (float) width / (float) height;

	glutReshapeWindow(640, 480);
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, 1.0f);
}

void timerFunc(int value)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutTimerFunc(1000 / 60, timerFunc, value);
	prevtime = currtime;
	currtime = glutGet(GLUT_ELAPSED_TIME);
	dt = (currtime - prevtime) / 1000.0f;

	if (dt > 0.016f)
		dt = 0.016f;

	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	glBindVertexArray(VAO);
		glUseProgram(shader.GetShader());
			player.Update(shader.GetShader());
			glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			for (auto bullet : BulletManager::GetBulletList())
			{
				bullet->Update(shader.GetShader());
				glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			}
			for (auto enemy : enemies)
			{
				enemy->Update(shader.GetShader());
				glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			}
		glUseProgram(0);
	glBindVertexArray(0);

	glUseProgram(textshader.GetShader());
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

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

		render_text("The Quick Brown Fox Jumps Over The Lazy Dog",
			-1 + sx, 1 - 240.0f * sy, sx, sy, 48);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	glUseProgram(0);

	glutSwapBuffers();
}

void CreateVAO()
{
	glGenBuffers(1, &vbo);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 300);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("SideScroller");

	glewExperimental = true;

	if (FT_Init_FreeType(&library))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	if (FT_New_Face(library, "AGENCYR.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		#if _DEBUG
			std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		#endif
	}

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	/*char writeBuffer[65536];
	FILE* pFile;

	fopen_s(&pFile,"config.json", "rb");
	if (pFile != NULL)
	{
		rapidjson::FileReadStream is(pFile, writeBuffer, sizeof(writeBuffer));
		rapidjson::Document d;
		d.ParseStream(is);
		fclose(pFile);

		const rapidjson::Value& second = d["second"];
		const rapidjson::Value& first = d["first"];

		for (rapidjson::Value::ConstMemberIterator itr = second[0].MemberBegin(); itr != second[0].MemberEnd(); ++itr) {
			std::cout << itr->name.GetString() << ":" << itr->value.GetType() << std::endl;
		}

		std::cout << std::endl << std::endl;

		for (rapidjson::Value::ConstMemberIterator itr = first[0].MemberBegin(); itr != first[0].MemberEnd(); ++itr) {
			std::cout << itr->name.GetString() << ":" << itr->value.GetType() << std::endl;
		}
	}
	else {
		std::cout << "config.json not found" << std::endl;
	}*/

	for (size_t i = 0; i <= 16; i++)
		enemies.push_back(new TestEnemy(32, 32, 32.0f + (i * 34.0f), 416.0f, 0.0f, 0.0f, 255.0f, 160.0f, 122.0f));

	player = Player(32, 32, 320.0f, 0.0f, 0.0f, 0.5f, 255.0f, 255.0f, 0.0f);
	shader = Shader("shader.vert", "shader.frag");
	textshader = Shader("textshader.vert", "textshader.frag");
	CreateVAO();

	glutTimerFunc(1000 / 60, timerFunc, 0);
	currtime = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(52.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
	glutMainLoop();
	return EXIT_SUCCESS;
}
