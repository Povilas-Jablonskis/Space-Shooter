#include "Renderer.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <freeglut/freeglut.h>

Renderer::Renderer()
{
	const GLuint indices[] =
	{
		// Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};

	constexpr GLfloat vertices[] = {
		// Positions    // Texture Coords
		0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
		0.0f, 1.0f, 0.0f, 1.0f, // Top Left
		1.0f, 1.0f, 1.0f, 1.0f, // Top Right
		1.0f, 0.0f, 1.0f, 0.0f // Bottom Right
	};

	glGenBuffers(1, &m_textVBO);
	glGenVertexArrays(1, &m_textVAO);
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_textVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);
	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

void Renderer::addShader(const std::string& name, const std::shared_ptr<Shader>& shader)
{
	m_shaders.insert_or_assign(name, shader);
}

void Renderer::draw(const std::vector<std::shared_ptr<Text>>& texts) const
{
	for (auto& text : texts)
	{
		draw(*text);
	}
}

void Renderer::draw(const Text& text) const
{
	const auto textShaderProgram = m_shaders.at("textshader")->get();

	glBindVertexArray(m_textVAO);
	glUseProgram(textShaderProgram);
	const auto colorUniform = glGetUniformLocation(textShaderProgram, "color");
	const auto projectionUniform = glGetUniformLocation(textShaderProgram, "projection");
	auto projection = glm::ortho(0.0f, static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)), 0.0f,
	                             static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)), 0.0f, 1.0f);

	glUniform4f(colorUniform, text.getColor().x / 255.0f, text.getColor().y / 255.0f, text.getColor().z / 255.0f,
	            text.getColor().a);
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, value_ptr(projection));

	auto& cache = text.getCachedCharacters();

	for (const auto& [textureID, vertices] : cache)
	{
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * 4, vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glUseProgram(0);
	glBindVertexArray(0);
}

void Renderer::draw(const Sprite& sprite) const
{
	const auto shaderProgram = m_shaders.at("shader")->get();

	glBindVertexArray(m_vao);
	glUseProgram(shaderProgram);
	const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
	const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

	const auto colorUniform = glGetUniformLocation(shaderProgram, "color");
	const auto spriteCoordinatesUniform = glGetUniformLocation(shaderProgram, "spriteCoordinates");
	const auto projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	const auto modelUniform = glGetUniformLocation(shaderProgram, "model");

	auto& textureRect = sprite.getTextureRect();
	auto& spriteSheet = sprite.getSpriteSheet();
	glBindTexture(GL_TEXTURE_2D, spriteSheet.getTexture());

	const auto spriteSheetSize = glm::vec2(spriteSheet.getWidth(), spriteSheet.getHeight());
	glUniform4f(spriteCoordinatesUniform, textureRect.x / spriteSheetSize.x, textureRect.y / spriteSheetSize.y,
	            textureRect.z / spriteSheetSize.x, textureRect.w / spriteSheetSize.y);
	glUniform4f(colorUniform, sprite.getColor().x / 255.0f, sprite.getColor().y / 255.0f,
	            sprite.getColor().z / 255.0f, sprite.getColor().a);
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE,
	                   value_ptr(glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, 0.0f, 1.0f)));
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, value_ptr(sprite.getTransform()));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glUseProgram(0);
	glBindVertexArray(0);
}
