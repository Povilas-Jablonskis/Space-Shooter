#include "UIElementBase.h"
#include "Application.h"

namespace Engine
{
	UIElementBase::UIElementBase()
	{
		InitFuncs();
	}

	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color) :
		width(_width), height(_height), position(_position), color(_color)
	{
		InitFuncs();
	}

	UIElementBase::~UIElementBase()
	{

	}

	void UIElementBase::InitFuncs()
	{
		OnHoverEnterFunc = []()
		{

		};

		OnHoverExitFunc = []()
		{

		};

		OnMouseClickFunc = []()
		{

		};

		OnMouseReleaseFunc = []()
		{

		};
	}

	void UIElementBase::Draw(InputManager* inputManager)
	{
		auto program = Application::GetShaderProgram("shader");
		
		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(Application::GetVAO());
			glUseProgram(program);
				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");

				glUniform2f(offsetLocation, position.x / windowwidth, position.y / windowheigth);
				glUniform2f(offsetLocation2, width / windowwidth, height / windowheigth);
				glUniform4f(offsetLocation3, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);
				glDrawElements(GL_TRIANGLES, (sizeof(Application::indices) / sizeof(*Application::indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}

	float UIElementBase::GetPosition(int index) const
	{
		return position[index];
	}

	int UIElementBase::GetSize(int index) const
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

	float UIElementBase::GetColor(int index) const
	{
		return color[index];
	}

	void UIElementBase::ChangeColor(float _color, int index)
	{
		color[index] = _color;
	}

	void UIElementBase::OnMouseClickDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->GetLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			OnMouseClickFunc();
	}

	void UIElementBase::OnMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		if (color.a == 0.0f) return;
		glm::vec2 lastMousePosition = inputManager->GetLastMousePosition();
		lastMousePosition.y -= glutGet(GLUT_WINDOW_HEIGHT);
		lastMousePosition.y *= -1;

		if (lastMousePosition.x >= position.x && lastMousePosition.x <= (position.x + width) && lastMousePosition.y <= position.y && lastMousePosition.y >= (position.y + height))
			OnMouseReleaseFunc();
	}
}