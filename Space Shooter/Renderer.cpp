#include "Renderer.h"

namespace Engine
{
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

	Renderer::~Renderer()
	{
		shaders.clear();
		glDeleteBuffers(1, &textVBO);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteVertexArrays(1, &textVAO);
		glDeleteVertexArrays(1, &vao);
	}

	GLuint Renderer::getShaderProgram(const std::string& name) const
	{
		auto tempShader = shaders.find(name);

		if (tempShader != shaders.end())
			return tempShader->second->getShader();

		return -1;
	}

	void Renderer::addShader(const std::string& name, std::shared_ptr<Shader> shader)
	{
		if (shaders.find(name) != shaders.end())
			return;

		shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(name, shader));
	}


	void Renderer::draw(std::vector<std::shared_ptr<RenderObject>> vector)
	{
		auto program = getShaderProgram("shader");
		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "renderMode");
		int offsetLocation3 = glGetUniformLocation(program, "animscX");
		int offsetLocation4 = glGetUniformLocation(program, "animscY");
		int offsetLocation5 = glGetUniformLocation(program, "curranim");
		int offsetLocation6 = glGetUniformLocation(program, "projection");
		int offsetLocation7 = glGetUniformLocation(program, "model");
		glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeigth, 0.0f, 1.0f);
		glBindVertexArray(vao);
			glUseProgram(program);
				for (auto element : vector)
				{
					if (element->getColor(3) == 0.0f) continue;

					auto texture = element->getTexture();

					glm::mat4 model;
					model = glm::translate(model, glm::vec3(element->getPosition(), 0.0f));

					model = glm::translate(model, glm::vec3(0.5f * element->getSize(0), 0.5f * element->getSize(1), 0.0f));
					model = glm::rotate(model, element->getRotationAngle(), element->getRotationAxis());
					model = glm::translate(model, glm::vec3(-0.5f * element->getSize(0), -0.5f * element->getSize(1), 0.0f));

					model = glm::scale(model, glm::vec3(element->getSize(0), element->getSize(1), 1.0f));

					glUniform4f(offsetLocation, element->getColor(0) / 255.0f, element->getColor(1) / 255.0f, element->getColor(2) / 255.0f, element->getColor(3));

					if (texture != nullptr)
					{
						glBindTexture(GL_TEXTURE_2D, texture->getTexture());

						glUniform1f(offsetLocation2, 1.0f);
						glUniform1f(offsetLocation3, texture->getCount().x);
						glUniform1f(offsetLocation4, texture->getCount().y);
						glUniform1f(offsetLocation5, (float)element->getCurrentFrame());
					}
					else
						glUniform1f(offsetLocation2, 0.0f);

					glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::shared_ptr<RenderObject> element)
	{
		auto program = getShaderProgram("shader");
		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "renderMode");
		int offsetLocation3 = glGetUniformLocation(program, "animscX");
		int offsetLocation4 = glGetUniformLocation(program, "animscY");
		int offsetLocation5 = glGetUniformLocation(program, "curranim");
		int offsetLocation6 = glGetUniformLocation(program, "projection");
		int offsetLocation7 = glGetUniformLocation(program, "model");
		glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeigth, 0.0f, 1.0f);
		glBindVertexArray(vao);
			glUseProgram(program);
				if (element->getColor(3) == 0.0f) return;

				auto texture = element->getTexture();

				glm::mat4 model;
				model = glm::translate(model, glm::vec3(element->getPosition(), 0.0f));

				model = glm::translate(model, glm::vec3(0.5f * element->getSize(0), 0.5f * element->getSize(1), 0.0f));
				model = glm::rotate(model, element->getRotationAngle(), element->getRotationAxis());
				model = glm::translate(model, glm::vec3(-0.5f * element->getSize(0), -0.5f * element->getSize(1), 0.0f));

				model = glm::scale(model, glm::vec3(element->getSize(0), element->getSize(1), 1.0f));

				glUniform4f(offsetLocation, element->getColor(0) / 255.0f, element->getColor(1) / 255.0f, element->getColor(2) / 255.0f, element->getColor(3));

				if (texture != nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, texture->getTexture());

					glUniform1f(offsetLocation2, 1.0f);
					glUniform1f(offsetLocation3, texture->getCount().x);
					glUniform1f(offsetLocation4, texture->getCount().y);
					glUniform1f(offsetLocation5, (float)element->getCurrentFrame());
				}
				else
					glUniform1f(offsetLocation2, 0.0f);

				glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::vector<std::shared_ptr<Text>> vector)
	{
		auto program = getShaderProgram("textshader");
		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "projection");
		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);
		glBindVertexArray(textVAO);
			glUseProgram(program);
				for (auto text : vector)
				{
					if (text->getColor(3) == 0.0f || text->getFont() == nullptr) continue;

					glUniform4f(offsetLocation, text->getColor(0) / 255.0f, text->getColor(1) / 255.0f, text->getColor(2) / 255.0f, text->getColor(3));
					glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

					auto cache = text->getCachedCharacters();

					for (std::vector<StructForTextCache>::iterator it = cache.begin(); it != cache.end(); it++)
					{
						// Render glyph texture over quad
						glBindTexture(GL_TEXTURE_2D, it->TextureID);
						// Update content of VBO memory
						glBindBuffer(GL_ARRAY_BUFFER, textVBO);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, &it->vertices[0]);
						glBindBuffer(GL_ARRAY_BUFFER, 0);
						// Render quad
						glDrawArrays(GL_TRIANGLES, 0, 6);
					}
				}
			glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::draw(std::shared_ptr<Text> text)
	{
		if (text->getColor(3) == 0.0f || text->getFont() == nullptr) return;
		auto program = getShaderProgram("textshader");
		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "projection");
		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);
		glBindVertexArray(textVAO);
			glUseProgram(program);
				if (text->getColor(3) == 0.0f || text->getFont() == nullptr) return;

				glUniform4f(offsetLocation, text->getColor(0) / 255.0f, text->getColor(1) / 255.0f, text->getColor(2) / 255.0f, text->getColor(3));
				glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

				auto cache = text->getCachedCharacters();

				for (std::vector<StructForTextCache>::iterator it = cache.begin(); it != cache.end(); it++)
				{
					// Render glyph texture over quad
					glBindTexture(GL_TEXTURE_2D, it->TextureID);
					// Update content of VBO memory
					glBindBuffer(GL_ARRAY_BUFFER, textVBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, &it->vertices[0]);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// Render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			glUseProgram(0);
		glBindVertexArray(0);
	}
}