#include "UIElement.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color, UIElement* _parent)
		: UIElementBase(_width, _height, _position, _color), parent(_parent)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::addText(std::shared_ptr<Text> text)
	{
		texts.push_back(text);
	}

	void UIElement::addUIElement(std::shared_ptr<UIElement> UIElement)
	{
		elements.push_back(UIElement);
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

	void UIElement::draw()
	{
		UIElementBase::draw();
		for (auto element : elements)
		{
			element.get()->draw();
		}
		for (auto text : texts)
		{
			text.get()->draw();
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

	void UIElement::setParent(UIElement* _parent)
	{
		parent = _parent;
	}

	UIElement* UIElement::getParent()
	{
		return parent;
	}
}