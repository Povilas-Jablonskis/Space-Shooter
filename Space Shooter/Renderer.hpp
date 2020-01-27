#ifndef rendererH
#define rendererH

#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <vector>
#include <memory>

#include "Animation.hpp"

namespace Engine
{
	class Text;
	class Shader;

	typedef std::pair<std::string, std::shared_ptr<Shader>> shader;

	class Renderer
	{
		public:
			Renderer();
			void addShader(const std::string&, const std::shared_ptr<Shader>&);
			inline const std::vector<shader>& getShaders() const { return m_shaders; }
			inline GLuint getVAO() const { return m_vao; }
			GLuint getShaderProgram(const std::string&);
			inline GLuint getTextVBO() const { return m_textVBO; }
			inline GLuint getTextVAO() const { return m_textVAO; }

			void draw(const std::vector<std::shared_ptr<Text>>&);
			void draw(const std::shared_ptr<Text>&);

			template <typename T>
			void draw(const std::vector<std::shared_ptr<T>>& ts)
			{
				for (auto it = ts.begin(); it != ts.end(); ++it)
				{
					draw(*it);
				}
			}

			template <typename T>
			void draw(const std::shared_ptr<T>& t)
			{
				auto program = getShaderProgram("shader");

				glBindVertexArray(getVAO());
				glUseProgram(program);
					auto windowWidth = static_cast<float>(glutGet(GLUT_WINDOW_WIDTH));
					auto windowHeight = static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT));

					auto offsetLocation = glGetUniformLocation(program, "color");
					auto offsetLocation2 = glGetUniformLocation(program, "renderMode");
					auto offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
					auto offsetLocation6 = glGetUniformLocation(program, "projection");
					auto offsetLocation7 = glGetUniformLocation(program, "model");
					glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, 0.0f, 1.0f);

					auto sprites = t->getAnimation()->getAnimation();

					glm::mat4 model(1.f);
					model = glm::translate(model, glm::vec3(t->getPosition(), 0.0f));

					model = glm::translate(model, glm::vec3(0.5f * t->getWidth(), 0.5f * t->getHeight(), 0.0f));
					model = glm::rotate(model, t->getRotationAngle(), t->getRotationAxis());
					model = glm::translate(model, glm::vec3(-0.5f * t->getWidth(), -0.5f * t->getHeight(), 0.0f));

					model = glm::scale(model, glm::vec3(t->getWidth(), t->getHeight(), 1.0f));

					glUniform4f(offsetLocation, t->getColor().x / 255.0f, t->getColor().y / 255.0f, t->getColor().z / 255.0f, t->getColor().a);

					if (t->getAnimation() != nullptr)
					{
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

					glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUseProgram(0);
				glBindVertexArray(0);
			}
		private:
			GLuint m_vbo, m_vao, m_ebo;
			GLuint m_textVBO;
			GLuint m_textVAO;
			std::vector<shader> m_shaders;
	};
}
#endif