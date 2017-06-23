#include "UIElement.h"
#include "Renderer.h"

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

	}

	void UIElement::AddText(std::string _text, int _fontsize, float x, float y, float r, float g, float b, float a, std::string font)
	{
		texts.push_back(new Text(_text, _fontsize, x, y, r, g, b, a, font));
	}

	void UIElement::AddText(Text* text)
	{
		texts.push_back(text);
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
			Renderer::Render(element);
		}
		for (auto text : texts)
		{
			Renderer::Render(text, this);
		}
	}
}