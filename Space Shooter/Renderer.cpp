#include "Renderer.h"

namespace Engine
{
	std::map<std::string, Shader*> Renderer::shaders;
	GLuint Renderer::indices[6] =
	{   // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	GLfloat Renderer::vertices[16]
	{
		// Positions    // Texture Coords
		1.0f, 1.0f, 1.0f, 1.0f, // Top Right
		1.0f, -1.0f, 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
		-1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};
	GLuint Renderer::VBO;
	GLuint Renderer::EBO;
	GLuint Renderer::VAO;

	void Renderer::Init()
	{
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

	void Renderer::AddShader(std::string name, Shader* shader){
		if (shaders.find(name) != shaders.end())
			return;
		shaders.insert(std::pair<std::string, Shader*>(name, shader));
	}

	void Renderer::Render(Player player)
	{
		if (shaders.find("shader") == shaders.end())
			return;

		Shader* shader = shaders.at("shader");

		glBindVertexArray(VAO);
			glUseProgram(shader->GetShader());
				player.Draw(shader->GetShader());
				glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);
		player.DrawOtherObjects();
	}

	void Renderer::Render(BaseGameObject* BaseGameObject)
	{
		if (shaders.find("shader") == shaders.end())
			return;

		Shader* shader = shaders.at("shader");

		glBindVertexArray(VAO);
			glUseProgram(shader->GetShader());
				BaseGameObject->Draw(shader->GetShader());
				glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);
	}

	void Renderer::Render(UIElement* UIElementBase)
	{
		if (shaders.find("shader") == shaders.end())
			return;

		Shader* shader = shaders.at("shader");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(VAO);
			glUseProgram(shader->GetShader());
				UIElementBase->Draw(shader->GetShader());
				glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);

		UIElementBase->DrawOtherElements();
	}

	void Renderer::Render(Text* text, UIElement* parent)
	{
		if (shaders.find("textshader") == shaders.end())
			return;

		Shader* shader = shaders.at("textshader");
		text->Draw(shader->GetShader(), parent);
	}
}