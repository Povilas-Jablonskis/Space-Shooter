#include "UIElement.h"
#include "Renderer.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement()
	{

	}

	UIElement::UIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color) 
		: UIElementBase(_width, _height, _position, _color)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::AddText(const std::string& _text, int _fontsize, glm::vec2 _position, glm::vec4 _color, const std::string& font)
	{
		texts.push_back(std::make_shared<Text>(_text, _fontsize, _position, _color, font));
	}

	void UIElement::AddUIElement(int _width, int _height, glm::vec2 _position, glm::vec4 _color)
	{
		elements.push_back(std::make_shared<UIElement>(_width, _height, _position, _color));
	}

	void UIElement::AddText(std::shared_ptr<Text> text)
	{
		texts.push_back(text);
	}

	void UIElement::AddUIElement(std::shared_ptr<UIElement> UIElement)
	{
		elements.push_back(UIElement);
	}

	void UIElement::Update()
	{
		for (auto text : texts)
		{
			text->Update();
		}
		for (auto element : elements)
		{
			element->Update();
		}
	}

	void UIElement::DrawOtherElements(Renderer renderer)
	{
		for (auto element : elements)
		{
			renderer.Render(element.get());
		}
		for (auto text : texts)
		{
			renderer.Render(text.get(), this);
		}
	}

	void UIElement::HideAllElements()
	{
		for (auto text : texts)
		{
			text->ChangeColor(0.0f, 3);
		}
		for (auto element : elements)
		{
			element->ChangeColor(0.0f, 3);
		}
	}

	void UIElement::ShowAllElements()
	{
		for (auto text : texts)
		{
			text->ChangeColor(1.0f, 3);
		}
		for (auto element : elements)
		{
			element->ChangeColor(1.0f, 3);
		}
	}
}