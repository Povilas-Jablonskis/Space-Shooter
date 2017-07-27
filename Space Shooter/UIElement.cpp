#include "UIElement.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color, std::string _parent, glm::vec2 _positionPerc, std::shared_ptr<Application> _application)
		: UIElementBase(_width, _height, _position, _color, _positionPerc, _application), parentMenu(_parent)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::update(float dt)
	{
		auto inputManager = application->getInputManager();
		UIElementBase::update(dt);
		for (auto text : texts)
		{
			text->update();
		}
		for (auto element : elements)
		{
			element->update(dt);
		}
	}

	void UIElement::draw()
	{
		auto renderer = application->getRender();
		auto program = renderer->getShaderProgram("shader");
		auto program2 = renderer->getShaderProgram("textshader");
		auto vao = renderer->getVAO();
		auto vao2 = renderer->getTextVAO();

		glUseProgram(program);
			glBindVertexArray(vao);
				UIElementBase::draw();
			glBindVertexArray(0);
		glUseProgram(0);
		glUseProgram(program2);
			glBindVertexArray(vao2);
				for (auto text : texts)
				{
					text.get()->draw();
				}
			glBindVertexArray(0);
		glUseProgram(0);

		for (auto element : elements)
		{
			element.get()->draw();
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

	void UIElement::onMouseClickDefaults()
	{
		for (auto text : texts)
		{
			text->onMouseClickDefaults();
		}
		for (auto element : elements)
		{
			element->onMouseClickDefaults();
		}

		UIElementBase::onMouseClickDefaults();
	}

	void UIElement::onMouseReleaseFuncDefaults()
	{
		for (auto text : texts)
		{
			text->onMouseReleaseFuncDefaults();
		}
		for (auto element : elements)
		{
			element->onMouseReleaseFuncDefaults();
		}

		UIElementBase::onMouseReleaseFuncDefaults();
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