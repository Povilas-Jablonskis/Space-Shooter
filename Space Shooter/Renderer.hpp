#ifndef rendererH
#define rendererH

#include "Animation.hpp"

#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <unordered_map>
#include <memory>

#include <freeglut/freeglut_std.h>

class Text;
class Shader;

class Renderer
{
public:
	Renderer();
	void addShader(const std::string&, const std::shared_ptr<Shader>&);
	const std::unordered_map<std::string, std::shared_ptr<Shader>>& getShaders() const { return m_shaders; }
	GLuint getVAO() const { return m_vao; }
	GLuint getShaderProgram(const std::string&) const;
	GLuint getTextVBO() const { return m_textVBO; }
	GLuint getTextVAO() const { return m_textVAO; }

	void draw(const std::vector<std::shared_ptr<Text>>&) const;
	void draw(const std::shared_ptr<Text>&) const;

	template <typename T>
	void draw(const std::vector<std::shared_ptr<T>>& ts) const
	{
		for (auto& t : ts)
		{
			draw(t);
		}
	}

	template <typename T>
	void draw(const std::shared_ptr<T>& t) const
	{
		const auto program = getShaderProgram("shader");

		glBindVertexArray(getVAO());
		glUseProgram(program);
		const auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
		const auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

		const auto offsetLocation = glGetUniformLocation(program, "color");
		const auto offsetLocation2 = glGetUniformLocation(program, "renderMode");
		const auto offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
		const auto offsetLocation6 = glGetUniformLocation(program, "projection");
		const auto offsetLocation7 = glGetUniformLocation(program, "model");
		auto projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, 0.0f, 1.0f);

		glm::mat4 model(1.f);
		model = translate(model, glm::vec3(t->getPosition(), 0.0f));

		model = translate(model, glm::vec3(0.5f * t->getWidth(), 0.5f * t->getHeight(), 0.0f));
		model = glm::rotate(model, t->getRotationAngle(), t->getRotationAxis());
		model = translate(model, glm::vec3(-0.5f * t->getWidth(), -0.5f * t->getHeight(), 0.0f));

		model = scale(model, glm::vec3(t->getWidth(), t->getHeight(), 1.0f));

		glUniform4f(offsetLocation, t->getColor().x / 255.0f, t->getColor().y / 255.0f, t->getColor().z / 255.0f, t->getColor().a);

		if (t->getAnimation() != nullptr)
		{
			auto sprites = t->getAnimation()->getAnimation();
			auto currentSprite = sprites.at(t->getCurrentFrame());
			glBindTexture(GL_TEXTURE_2D, t->getAnimation()->getSpriteSheetTexture());

			glUniform1f(offsetLocation2, 1.0f);

			auto spriteSheetSize = glm::vec2(t->getAnimation()->getSpriteSheetWidth(), t->getAnimation()->getSpriteSheetHeight());
			glUniform4f(offsetLocation3, currentSprite.x / spriteSheetSize.x, currentSprite.y / spriteSheetSize.y, currentSprite.z / spriteSheetSize.x, currentSprite.w / spriteSheetSize.y);
		}
		else
		{
			glUniform1f(offsetLocation2, 0.0f);
		}

		glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, value_ptr(projection));
		glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glUseProgram(0);
		glBindVertexArray(0);
	}
private:
	GLuint m_vbo{}, m_vao{}, m_ebo{};
	GLuint m_textVBO{};
	GLuint m_textVAO{};
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
#endif