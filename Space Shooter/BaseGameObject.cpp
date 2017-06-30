#include "BaseGameObject.h"
#include "Renderer.h"

namespace Engine
{
	BaseGameObject::BaseGameObject()
	{

	}

	BaseGameObject::BaseGameObject(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color)
		: width(_width), height(_height), position(_position), velocity(_velocity), color(_color) {}

	BaseGameObject::~BaseGameObject()
	{

	}

	void BaseGameObject::Draw(GLuint program)
	{
		float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "givenposition");
		int offsetLocation2 = glGetUniformLocation(program, "size");
		int offsetLocation3 = glGetUniformLocation(program, "color");

		glUniform2f(offsetLocation, position[0] / _width, position[1] / _height);
		glUniform2f(offsetLocation2, width / _width, height / _height);
		glUniform4f(offsetLocation3, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f);
	}

	const float BaseGameObject::GetPosition(int index)
	{
		return position[index];
	}

	const int BaseGameObject::GetSize(int index)
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

	const float BaseGameObject::GetColor(int index)
	{
		return color[index];
	}
}