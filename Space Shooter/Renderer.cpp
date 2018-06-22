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
		for (auto shader : shaders)
		{
			if (shader.first == name)
				return shader.second->getShader();
		}

		return -1;
	}

	void Renderer::addShader(const std::string& name, std::shared_ptr<Shader> _shader)
	{
		for (auto theShader : shaders)
		{
			if (theShader.first == name)
				return;
		}

		shaders.push_back(std::move(shader(name, _shader)));
	}
}