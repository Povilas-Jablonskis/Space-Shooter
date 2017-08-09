#ifndef _rendererH
#define _rendererH

#include <map>
#include <memory>
#include <algorithm>

#include "Shader.h"
#include "Text.h"
#include "RenderObject.h"
#include "Font.h"

namespace Engine
{
	class Renderer
	{
		public:
			~Renderer();
			Renderer();
			void addShader(const std::string& name, std::shared_ptr<Shader> shader);
			inline GLuint getVAO() const { return vao; }
			GLuint getShaderProgram(const std::string&) const;
			inline GLuint getTextVBO() const { return textVBO; }
			inline GLuint getTextVAO() const { return textVAO; }
			template <typename T>
			void draw(std::vector<std::shared_ptr<T>> vector)
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
			template <typename T>
			void draw(std::shared_ptr<T> element)
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
			void draw(std::vector<std::shared_ptr<Text>>);
			void draw(std::shared_ptr<Text>);
		private:
			GLuint vbo, vao, ebo;
			GLuint textVBO;
			GLuint textVAO;
			std::map<std::string, std::shared_ptr<Shader>> shaders;
	};
}
#endif