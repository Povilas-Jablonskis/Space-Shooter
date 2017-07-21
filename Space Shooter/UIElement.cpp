#include "UIElement.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color, std::string _parent, glm::vec2 _positionPerc)
		: UIElementBase(_width, _height, _position, _color, _positionPerc), parentMenu(_parent)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::update(InputManager* inputManager, float dt)
	{
		UIElementBase::update(inputManager, dt);
		for (auto text : texts)
		{
			text->update(inputManager);
		}
		for (auto element : elements)
		{
			element->update(inputManager, dt);
		}
	}

	void UIElement::draw(GLuint program, GLuint program2, GLuint vao, GLuint vao2, GLuint vbo2)
	{
		glUseProgram(program);
			glBindVertexArray(vao);
				UIElementBase::draw(program);
			glBindVertexArray(0);
		glUseProgram(0);
		glUseProgram(program2);
			glBindVertexArray(vao2);
				for (auto text : texts)
				{
					text.get()->draw(program2, vbo2);
				}
			glBindVertexArray(0);
		glUseProgram(0);

		for (auto element : elements)
		{
			element.get()->draw(program, program2, vao, vao2, vbo2);
		}
	}

	void UIElement::hideAllElements()
	{
		for (auto text : texts)
		{
			text->changeColor(0.0f, 3);
		}
		for (auto element : elements)
		{
			element->hideAllElements();
		}
	}

	void UIElement::showAllElements()
	{
		for (auto text : texts)
		{
			text->changeColor(1.0f, 3);
		}
		for (auto element : elements)
		{
			element->showAllElements();
		}
	}

	void UIElement::onMouseClickDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->onMouseClickDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->onMouseClickDefaults(inputManager);
		}

		UIElementBase::onMouseClickDefaults(inputManager);
	}

	void UIElement::onMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->onMouseReleaseFuncDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->onMouseReleaseFuncDefaults(inputManager);
		}

		UIElementBase::onMouseReleaseFuncDefaults(inputManager);
	}

	void UIElement::onHoverEnterFuncDefaults()
	{

	}

	void UIElement::onHoverExitFuncDefaults()
	{

	}

	void UIElement::fixPosition(UIElementBase* _parent)
	{
		UIElementBase::fixPosition(_parent);

		for (auto text : texts)
		{
			text->fixPosition(this);
		}
		for (auto element : elements)
		{
			element->fixPosition(this);
		}
	}
}