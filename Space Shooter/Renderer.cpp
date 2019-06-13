#include "Renderer.h"

namespace Engine
{
	Renderer::~Renderer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &textVBO);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &textVAO);
		glDeleteVertexArrays(1, &vao);
	}

	Renderer::Renderer()
	{
		GLuint indices[] =
		{   // Note that we start from 0!
			0, 1, 3,  // First Triangle
			1, 2, 3   // Second Triangle
		};

		GLfloat vertices[] = {
			// Positions    // Texture Coords
			0.0f, 0.0f,		0.0f, 0.0f, // Bottom Left
			0.0f, 1.0f,		0.0f, 1.0f, // Top Left
			1.0f, 1.0f,		1.0f, 1.0f, // Top Right
			1.0f, 0.0f,		1.0f, 0.0f  // Bottom Right
		};

		glGenBuffers(1, &textVBO);
		glGenVertexArrays(1, &textVAO);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(textVAO);

		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	}

	GLuint Renderer::getShaderProgram(std::string name)
	{
		for (auto shader : shaders)
		{
			if (shader.first == name)
				return shader.second->getShader();
		}

		return -1;
	}

	void Renderer::addShader(std::string name, std::shared_ptr<Shader> _shader)
	{
		for (auto theShader : shaders)
		{
			if (theShader.first == name)
				return;
		}

		shaders.push_back(shader(name, _shader));
	}

	void Renderer::draw(std::shared_ptr<Text> text, GLuint program)
	{
		auto offsetLocation = glGetUniformLocation(program, "color");
		auto offsetLocation2 = glGetUniformLocation(program, "projection");
		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);

		glUniform4f(offsetLocation, text->getColor().x / 255.0f, text->getColor().y / 255.0f, text->getColor().z / 255.0f, text->getColor().a);
		glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

		auto cache = text->getCachedCharacters();

		for (auto it = cache.begin(); it != cache.end(); it++)
		{
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, it->first);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, getTextVBO());
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, &it->second[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void Renderer::draw(std::vector<std::shared_ptr<Text>> texts)
	{
		auto program = getShaderProgram("textshader");
		glBindVertexArray(getTextVAO());
		glUseProgram(program);
			for (auto text : texts)
			{
				draw(text, program);
			}
		glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::shared_ptr<Menu> menu)
	{
		auto program = getShaderProgram("shader");
		auto program2 = getShaderProgram("textshader");

		auto uiElements = menu->getElements();
		auto texts = menu->getTexts();

		glBindVertexArray(getTextVAO());
		glUseProgram(program2);
			for (auto text : texts)
			{
				draw(text, program2);
			}
		glUseProgram(0);
		glBindVertexArray(0);

		glBindVertexArray(getVAO());
		glUseProgram(program);
			for (auto uiElement : uiElements)
			{
				draw(uiElement, program);
			}
		glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::vector<addon> addons)
	{
		auto program = getShaderProgram("shader");

		glBindVertexArray(getVAO());
		glUseProgram(program);
			for (auto addon : addons)
			{
				draw(addon.second, program);
			}
		glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::vector<uiPlayerElement> notifications)
	{
		auto program = getShaderProgram("textshader");

		glBindVertexArray(getTextVAO());
		glUseProgram(program);
			for (auto notification : notifications)
			{
				draw(notification.second, program);
			}
		glUseProgram(0);
		glBindVertexArray(0);
	}
}