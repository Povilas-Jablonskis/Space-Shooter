#include "UIElementBase.h"

namespace Engine
{
	UIElementBase::UIElementBase()
	{

	}

	UIElementBase::UIElementBase(int _width, int _height, float x, float y, float r, float g, float b, float a)
	{
		width = _width;
		height = _height;
		position[0] = x;
		position[1] = y;
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
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

	void UIElementBase::Update()
	{

	}

	float UIElementBase::GetPosition(int index)
	{
		return position[index];
	}

	int UIElementBase::GetSize(int index)
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

	void UIElementBase::ChangeColor(float _color, int index)
	{
		color[index] = _color;
	}

	float UIElementBase::GetColor(int index)
	{
		return color[index];
	}
}