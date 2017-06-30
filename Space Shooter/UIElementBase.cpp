#include "UIElementBase.h"

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

	void UIElementBase::Draw(GLuint program)
	{
		float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "givenposition");
		int offsetLocation2 = glGetUniformLocation(program, "size");
		int offsetLocation3 = glGetUniformLocation(program, "color");

		glUniform2f(offsetLocation, (_width * position[0]) / _width, (_height * position[1]) / _height);
		glUniform2f(offsetLocation2, width / _width, height / _height);
		glUniform4f(offsetLocation3, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3]);
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