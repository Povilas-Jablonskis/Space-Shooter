#include "UIElementBase.h"
#include "Application.h"

namespace Engine
{
	UIElementBase::UIElementBase()
	{

	}

	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color) :
		width(_width), height(_height), position(_position), color(_color)
	{

	}

	UIElementBase::~UIElementBase()
	{

	}

	void UIElementBase::Draw()
	{
		auto program = Application::GetShaderProgram("shader");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(Application::GetVAO());
			glUseProgram(program);
				float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");

				glUniform2f(offsetLocation, (_width * position[0]) / _width, (_height * position[1]) / _height);
				glUniform2f(offsetLocation2, width / _width, height / _height);
				glUniform4f(offsetLocation3, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3]);
				glDrawElements(GL_TRIANGLES, (sizeof(Application::indices) / sizeof(*Application::indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}

	const float UIElementBase::GetPosition(int index)
	{
		return position[index];
	}

	const int UIElementBase::GetSize(int index)
	{
		switch (index)
		{
		case 0:
		{
			return width;
		}
		case 1:
		{
			return height;
		}
		default:
		{
			return NULL;
		}
		}
	}

	const float UIElementBase::GetColor(int index)
	{
		return color[index];
	}

	void UIElementBase::ChangeColor(float _color, int index)
	{
		color[index] = _color;
	}
}