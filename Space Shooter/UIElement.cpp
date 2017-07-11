#include "UIElement.h"
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

	void UIElement::AddText(std::shared_ptr<Text> text)
	{
		texts.push_back(text);
	}

	void UIElement::AddUIElement(std::shared_ptr<UIElement> UIElement)
	{
		elements.push_back(UIElement);
	}

	void UIElement::Update(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->Update(inputManager);
		}
		for (auto element : elements)
		{
			element->Update(inputManager);
		}
	}

	void UIElement::Draw(InputManager* inputManager)
	{
		UIElementBase::Draw(inputManager);
		for (auto element : elements)
		{
			element.get()->Draw(inputManager);
		}
		for (auto text : texts)
		{
			text.get()->Draw();
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

	void UIElement::OnMouseClickDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->OnMouseClickDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->OnMouseClickDefaults(inputManager);
		}

		UIElementBase::OnMouseClickDefaults(inputManager);
	}

	void UIElement::OnMouseReleaseFuncDefaults(InputManager* inputManager)
	{
		for (auto text : texts)
		{
			text->OnMouseReleaseFuncDefaults(inputManager);
		}
		for (auto element : elements)
		{
			element->OnMouseReleaseFuncDefaults(inputManager);
		}

		UIElementBase::OnMouseReleaseFuncDefaults(inputManager);
	}

	void UIElement::OnHoverEnterFuncDefaults()
	{

	}

	void UIElement::OnHoverExitFuncDefaults()
	{

	}
}