#include "UIElementBase.h"
#include "Application.h"

namespace Engine
{
	UIElementBase::UIElementBase()
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

	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color, UIElementBase* _parent) :
		width(_width), height(_height), position(_position), color(_color), parent(_parent)
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

	UIElementBase::UIElementBase(int _width, int _height, glm::vec2 _position, glm::vec4 _color) :
		width(_width), height(_height), position(_position), color(_color), parent(nullptr)
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

	UIElementBase::~UIElementBase()
	{

	}

	void UIElementBase::Draw(InputManager* inputManager)
	{
		auto program = Application::GetShaderProgram("shader");
		
		float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));
		//auto _parent = GetFirstParent();
		//glm::vec2 bbox = glm::vec2(_width * position.x, _height * position.y);

		/*if (parent != NULL)
		{
			float startx = parent->GetPosition(0) * _width;
			float endx = startx + parent->GetSize(0);
			float starty = parent->GetPosition(1) * _height;
			float endy = starty + parent->GetSize(1);

			float xdiff = endx - startx;
			float ydiff = endy - starty;

			bbox.x = ((startx + (xdiff * position.x)));
			bbox.y = ((starty + (ydiff * position.y)));

			if (position.x == 0.0f)
				x = -1 + startx * sx;
			else
				x = -1 + ((startx + (xdiff * position.x))) * sx;

			if (position.y == 0.0f)
				y = -1 + starty * sy;
			else
				y = -1 + ((starty + (ydiff * position.y))) * sy;
		}*/

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(Application::GetVAO());
			glUseProgram(program);
				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");

				glUniform2f(offsetLocation, position.x / _width, position.y / _height);
				glUniform2f(offsetLocation2, width / _width, height / _height);
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

	void UIElementBase::ChangeParent(UIElementBase* _parent)
	{
		parent = _parent;
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

	UIElementBase* UIElementBase::GetParent() const
	{
		return parent;
	}
}