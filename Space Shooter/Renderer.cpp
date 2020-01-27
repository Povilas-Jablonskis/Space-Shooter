#include "Renderer.hpp"
#include "Shader.hpp"
#include "Text.hpp"

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

		glGenBuffers(1, &m_textVBO);
		glGenVertexArrays(1, &m_textVAO);
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_textVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	}

	GLuint Renderer::getShaderProgram(const std::string& name)
	{
		auto it = std::find_if(getShaders().begin(), getShaders().end(), [name](auto shaderName) { return shaderName.first == name; });

		return it != getShaders().end() ? it->second->getShader() : -1;
	}

	void Renderer::addShader(const std::string& name, const std::shared_ptr<Shader>& t_shader)
	{
		for (auto it = getShaders().begin(); it != getShaders().end(); ++it)
		{
			if (it->first == name)
			{
				return;
			}
		}

		m_shaders.push_back(shader(name, t_shader));
	}

	void Renderer::draw(const std::vector<std::shared_ptr<Text>>& texts)
	{
		for (auto it = texts.begin(); it != texts.end(); ++it)
		{
			draw(*it);
		}
	}

	void Renderer::draw(const std::shared_ptr<Text>& text)
	{
		auto program = getShaderProgram("textshader");

		glBindVertexArray(getTextVAO());
		glUseProgram(program);
			auto offsetLocation = glGetUniformLocation(program, "color");
			auto offsetLocation2 = glGetUniformLocation(program, "projection");
			glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)), 0.0f, static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)), 0.0f, 1.0f);

			glUniform4f(offsetLocation, text->getColor().x / 255.0f, text->getColor().y / 255.0f, text->getColor().z / 255.0f, text->getColor().a);
			glUniformMatrix4fv(offsetLocation2, 1, GL_FALSE, glm::value_ptr(projection));

			auto cache = text->getCachedCharacters();

			for (auto it = cache.begin(); it != cache.end(); ++it)
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
		glUseProgram(0);
		glBindVertexArray(0);
	}
}