#ifndef _rendererH
#define _rendererH

#include <algorithm>

#include "Shader.h"
#include "Menu.h"
#include "BaseGameObject.h"

namespace Engine
{
	typedef std::pair<std::string, std::shared_ptr<Shader>> shader;
	typedef std::pair<std::string, std::shared_ptr<BaseGameObject>> addon;
	typedef std::pair<std::string, std::shared_ptr<Text>> uiPlayerElement;

	class Renderer
	{
		public:
			~Renderer();
			Renderer();
			void addShader(std::string name, std::shared_ptr<Shader> shader);
			inline std::vector<shader> getShaders() { return shaders; }
			inline GLuint getVAO() { return vao; }
			GLuint getShaderProgram(std::string);
			inline GLuint getTextVBO() { return textVBO; }
			inline GLuint getTextVAO() { return textVAO; }
			void draw(std::vector<std::shared_ptr<Text>>);
			void draw(std::shared_ptr<Menu>);
			void draw(std::vector<addon>);
			void draw(std::vector<uiPlayerElement>);
			template <typename T>
			void draw(std::vector<std::shared_ptr<T>> ts)
			{
				auto program = getShaderProgram("shader");
				glBindVertexArray(getVAO());
				glUseProgram(program);
					for (auto t : ts)
					{
						draw(t, program);
					}
				glUseProgram(0);
				glBindVertexArray(0);
			}
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::vector<shader> shaders;
			void draw(std::shared_ptr<Text>, GLuint);
			template <typename T>
			void draw(std::shared_ptr<T> t, GLuint program)
			{
				auto windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				auto windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				auto offsetLocation = glGetUniformLocation(program, "color");
				auto offsetLocation2 = glGetUniformLocation(program, "renderMode");
				auto offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
				auto offsetLocation6 = glGetUniformLocation(program, "projection");
				auto offsetLocation7 = glGetUniformLocation(program, "model");
				glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeigth, 0.0f, 1.0f);

				auto animation = t->getAnimation();
				auto sprites = animation->getAnimation();

				glm::mat4 model;
				model = glm::translate(model, glm::vec3(t->getPosition(), 0.0f));

				model = glm::translate(model, glm::vec3(0.5f * t->getWidth(), 0.5f * t->getHeight(), 0.0f));
				model = glm::rotate(model, t->getRotationAngle(), t->getRotationAxis());
				model = glm::translate(model, glm::vec3(-0.5f * t->getWidth(), -0.5f * t->getHeight(), 0.0f));

				model = glm::scale(model, glm::vec3(t->getWidth(), t->getHeight(), 1.0f));

				glUniform4f(offsetLocation, t->getColor().x / 255.0f, t->getColor().y / 255.0f, t->getColor().z / 255.0f, t->getColor().a);

				if (animation != nullptr)
				{
					auto currentSprite = sprites->at(t->getCurrentFrame());
					glBindTexture(GL_TEXTURE_2D, animation->getSpriteSheetTexture());

					glUniform1f(offsetLocation2, 1.0f);

					auto spriteSheetSize = glm::vec2(animation->getSpriteSheetSize(0), animation->getSpriteSheetSize(1));
					glUniform4f(offsetLocation3, currentSprite.x / spriteSheetSize.x, currentSprite.y / spriteSheetSize.y, currentSprite.z / spriteSheetSize.x, currentSprite.w / spriteSheetSize.y);
				}
				else
				{
					glUniform1f(offsetLocation2, 0.0f);
				}

				glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
				glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
	};
}
#endif