#include "UIElement.h"
#include "Renderer.h"
#include <algorithm>

namespace Engine
{
	UIElement::UIElement()
	{

	}

	UIElement::UIElement(int _width, int _height, float x, float y, float r, float g, float b, float a) : UIElementBase(_width, _height, x, y, r, g, b, a)
	{

	}

	UIElement::~UIElement()
	{
		texts.clear();
		elements.clear();
	}

	void UIElement::AddText(std::string _text, int _fontsize, float x, float y, float r, float g, float b, float a, std::string font)
	{
		texts.push_back(std::make_shared<Text>(_text, _fontsize, x, y, r, g, b, a, font));
	}

	void UIElement::AddUIElement(int _width, int _height, float x, float y, float r, float g, float b, float a)
	{
		elements.push_back(std::make_shared<UIElement>(_width, _height, x, y, r, g, b, a));
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

	void UIElement::DrawOtherElements()
	{
		for (auto element : elements)
		{
			Renderer::Render(element.get());
		}
		for (auto text : texts)
		{
			Renderer::Render(text.get(), this);
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